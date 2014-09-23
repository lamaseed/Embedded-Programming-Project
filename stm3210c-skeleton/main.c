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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "setup.h"

#include "stm32f10x_conf.h"
#include "stm3210c_eval_ioe.h"
#include "semphr.h"




#include "global.h"
#include "UART.h"
#include "LCD.h"
#include "CU.h"
/*-----------------------------------------------------------*/


Room_Measurements room_measurements[] = { {0,0},{0,0} }; // 0.o

Room Floor = {
	room_measurements,
	NULL,
	100
};
/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART2_Configuration(void);

/* Public function prototypes -----------------------------------------------*/
void getCurrentTemperature(char *Array) {
   getTemperature(&Floor, Array);
}
void getCurrentHumidity(char *Array) {
   getHumidity(&Floor, Array);
}
void getDesiredTemperature(char *Array) {
   getDesired(Array);
}

int main(void)
{
  prvSetupHardware();
  IOE_Config();
	
	RCC_Configuration();
	GPIO_Configuration();
  USART2_Configuration();
	NVIC_Configuration();
	
	initDisplay();
	setupButtons();
	setupComTasks(&Floor);
	setupControlTask();
	
  printf("Setup complete ");  // this is redirected to the display
  vTaskStartScheduler();
	
  assert_param(0);
}
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* The interrupt controller is configured
   * to jump to the USART2_IRQHandler() function
   * if the USART2 receive interrupt occurs */

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;					  // we want to configure the USART2 interrupts
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		// this sets the priority group of the USART2 interrupts
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					// this sets the sub priority inside the group
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							// the USART2 interrupts are globally enabled
  NVIC_Init(&NVIC_InitStructure);															// the properties are passed to the NVIC_Init function which takes care of the low level stuff
	
}

/**************************************************************************************/

void RCC_Configuration(void)
{
  /* --------------------------- System Clocks Configuration -----------------*/
  /* USART2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* GPIOD, AFIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
}

/**************************************************************************************/

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*-------------------------- GPIO Configuration ----------------------------*/
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  // PD.05 USART2.TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; // PD.06 USART2.RX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  /* Re-map USART, as USART2 is used as alternate pins on PD5/6, requires AFIO */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
}

/**************************************************************************************/

void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 4800 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  USART_InitStructure.USART_BaudRate = 4800;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  /* USART configuration */
  USART_Init(USART2, &USART_InitStructure);
 
  /* Enable the USART2 */
  USART_Cmd(USART2, ENABLE);

  /* Here the USART2 receive interrupt is enabled */	
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

/**************************************************************************************/
