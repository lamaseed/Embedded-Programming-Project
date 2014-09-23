/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Project Authors:
 * 	Zacharopoulos 		Georgios
 * 	Karkanis 					Iosif
 *	Ntounas						Georgios
 *	Stamatakopoulos		Panagiotis
 */

/**
 * The UART module, which is responsible for the 
 * communication between the ARM and the AVR
 */
 
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h" 
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "global.h"
#include "UART.h"

/*--------------USART2 IRQ_HANDLER---------------*/
 
#define LINEMAX 100 // Maximal allowed/expected line length
 
volatile char line_buffer[LINEMAX + 1]; // Holding buffer with space for terminating NUL
volatile int line_valid = 0;

xSemaphoreHandle ComToken;


/*--------------USART2 IRQ_HANDLER---------------*/


xSemaphoreHandle ComToken;

void USART2_IRQHandler(void){
	Events TxBuffer;
	int TxEvent;
	static char rx_buffer[LINEMAX];   // Local holding buffer to build line
  static int rx_index = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){  // Received character?
		
		char rx =  USART_ReceiveData(USART2);
		
		rx_buffer[rx_index] = rx;
		rx_index++;
		if (rx_index == 6) // "temp25" || "humi25"
		{
			memcpy((void *)line_buffer, rx_buffer, rx_index);
			xSemaphoreGive(ComToken);
			rx_index = 0;
		}
		if (rx_buffer[0] != 'h' && rx_buffer[0] != 't'){ //this is a check to avoid mistakes
			printf("UART ERROR ");
			rx_index = 0;
		}
	}
	
	if (USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
  {
		if ( xQueueReceiveFromISR(TxQueue,&TxBuffer,0) == pdTRUE ){
			TxEvent = (int)TxBuffer;
		
			switch (TxEvent){
				
				case HEATER_ON:
					USART_SendData	(	USART2, '0');  // Open HEATER
				break;
				
				case HEATER_OFF:
					USART_SendData	(	USART2, '1');  // Close HEATER
				break;
				
				case FAN_ON:			
					USART_SendData	(	USART2, '2');  // Open FAN
				break;
				
				case FAN_OFF:
					USART_SendData	(	USART2, '3');  // Close FAN
				break;
							
			}
			while(USART_GetITStatus(USART2, USART_IT_TXE) == RESET);
		}else
		{
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE); // Suppress interrupt when empty
		}
  }
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}
/*--------------COMMUNICATION TASK---------------*/
static void UpdateTask(void *params)
{
	int ret;
	Room Floor = *((Room*)params);
  for(;;)
  {  
    xSemaphoreTake(ComToken, portMAX_DELAY);

		ret = strncmp((void *)line_buffer, "temp", 4);
		
		if (ret == 0){
			UpdateTemperature(&Floor);
		}
		ret = strncmp((void *)line_buffer, "humi", 4);
		
		if (ret == 0){
			UpdateHumidity(&Floor);
		}
		
	}
}
/*--------------COMMUNICATION TASK---------------*/


static void SendingTask(void *params)
{
	Events TxBuffer;
	//int TxEvent;
  for(;;)
  {  
		xQueuePeek(TxQueue,&TxBuffer,portMAX_DELAY);
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); // Enable when something to send

	}
}
/*--------------------------------------------*/

xQueueHandle TxQueue;

void setupComTasks(Room *Floor) {
  vSemaphoreCreateBinary(ComToken);
	TxQueue = xQueueCreate(32, sizeof(Events));
	xSemaphoreTake(ComToken, 0);
	Floor->lock =	xSemaphoreCreateMutex();
	
	
	xTaskCreate(SendingTask, "SendingTask",
                    100,(void*)Floor,
										3, NULL);
	
  xTaskCreate(UpdateTask, "UpdateTask",
                    100,(void*)Floor,
										3, NULL);
 
}

/*--------------------------------------------*/
void UpdateTemperature(Room *Floor) {

  	xSemaphoreTake(Floor->lock, portMAX_DELAY);
			Floor->Measurements->temperature[0] = line_buffer[4];
			Floor->Measurements->temperature[1] = line_buffer[5];
		xSemaphoreGive(Floor->lock);

}
void UpdateHumidity(Room *Floor) {

  	xSemaphoreTake(Floor->lock, portMAX_DELAY);
			Floor->Measurements->humidity[0] = line_buffer[4];
			Floor->Measurements->humidity[1] = line_buffer[5];
		xSemaphoreGive(Floor->lock);

}
void getTemperature(Room *Floor,char *Array) {
	
		xSemaphoreTake(Floor->lock, portMAX_DELAY);
			memcpy(Array,Floor->Measurements->temperature,2);
		xSemaphoreGive(Floor->lock);
	
}
void getHumidity(Room *Floor,char *Array) {
	
		xSemaphoreTake(Floor->lock, portMAX_DELAY);
			memcpy(Array,Floor->Measurements->humidity,2);
		xSemaphoreGive(Floor->lock);
	
}
/**************************************************************************************/
