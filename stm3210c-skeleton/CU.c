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
 * Main file of the system; module setup and initialisation takes
 * place here
 */
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_usart.h"

#include "global.h"
#include "CU.h"

volatile Events state = AUTO_OFF;
Events Buffer;


static void ControlTask(void *params) {
	char Current[2];
	Events fan_on = FAN_ON,fan_off = FAN_OFF ,
				 heater_on = HEATER_ON, heater_off = HEATER_OFF;
	char Desired_Temp[2] = {'2','5'};
	int Event, ret, heater_is_on = 0, fan_is_on = 0;
	portTickType lastWakeTime = xTaskGetTickCount();
	
  for (;;) {
		
    if (xQueueReceive(EventQueue, &Buffer, 0) == pdTRUE ){ // See if a new event has happened
		  Event = (int)Buffer;
			switch (Event){

				case AUTO_OFF:
					state = AUTO_OFF;
					break;
				
				case AUTO_ON:
					state = AUTO_ON;
					break;

				case FAN_ON:
					if (!fan_is_on){
						fan_is_on = 1;
						xQueueSend( TxQueue, &fan_on,  0);
					}	
					break;

				case FAN_OFF:
					if (fan_is_on){
						fan_is_on = 0;
						xQueueSend( TxQueue, &fan_off,  0);
					}	
					break;

				case HEATER_ON:
					if (!heater_is_on){
						heater_is_on = 1;
						xQueueSend( TxQueue, &heater_on,  0);
					}
					break;

				case HEATER_OFF:
					if (heater_is_on){
						heater_is_on = 0;
						xQueueSend( TxQueue, &heater_off,  0);
					}			
					break;
				
				case NEW_TEMP:
					printf("NEW_TEMP");
				  getDesiredTemperature((void *)Desired_Temp);
					break;
		}	
	}
	
	if (state)
	{
		
		getCurrentTemperature(Current);
		ret = strncmp(Desired_Temp, Current, 2);
		
		if (ret == 0){
			if (fan_is_on){
			  fan_is_on = 0;
				xQueueSend( TxQueue, &fan_off,  0);
			}	
			if (heater_is_on){
			  heater_is_on = 0;
				xQueueSend( TxQueue, &heater_off,  0);
			}			
		}
		if (ret > 0){    //Desired_Temp > Current

			
			if (fan_is_on){
			  fan_is_on = 0;
				xQueueSend( TxQueue, &fan_off,  0);
			}	
			if (!heater_is_on){
			  heater_is_on = 1;
				xQueueSend( TxQueue, &heater_on,  0);
			}
		}		
		if (ret < 0){    //Desired_Temp < Current
			
			if (heater_is_on){
			  heater_is_on = 0;
				xQueueSend( TxQueue, &heater_off,  0);
			}
			if (!fan_is_on){
			  fan_is_on = 1;
				xQueueSend( TxQueue, &fan_on,  0);
			}			
		}
	}else
	{
		
	}
	
	vTaskDelayUntil(&lastWakeTime, 1000 / portTICK_RATE_MS);
}
}

void setupControlTask(void) {
  xTaskCreate(ControlTask, "ControlTask", 100, NULL, 2, NULL);
}
