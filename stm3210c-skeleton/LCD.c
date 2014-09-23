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

 */
#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x.h"
#include "stm3210c_eval_lcd.h"
#include "stm3210c_eval_ioe.h"
#include "stm32f10x_usart.h"
#include "LCD.h"
#include "global.h"
#include "Screens-Functions.c"

/*-----------------------------------------------------------*/

xQueueHandle printQueue;
xSemaphoreHandle lcdLock;


volatile SCREEN_TypeDef Current_Screen;
volatile char Desired_Temp1 = '2';
volatile char Desired_Temp2 = '5';
/****************************Code about the project****************************/
/**
 * Display stdout on the display
 */


static void printTask(void *params) {
  unsigned char str[21] = "                    ";
  portTickType lastWakeTime = xTaskGetTickCount();
  int i;

  for (;;) {
    xSemaphoreTake(lcdLock, portMAX_DELAY);
    LCD_SetTextColor(White);
    LCD_DisplayStringLine(Line9, str);
    xSemaphoreGive(lcdLock);

    for (i = 0; i < 19; ++i)
	  str[i] = str[i+1];

    if (!xQueueReceive(printQueue, str + 19, 0))
	  str[19] = ' ';

	vTaskDelayUntil(&lastWakeTime, 100 / portTICK_RATE_MS);
  }
}

/*-----------------------------------------------------------*/
 static void DisplayTask(void *params) {   //show current temperature
	char Current[2];
  for (;;) {
		switch (Current_Screen){
			
			case HOME:
					getCurrentTemperature(Current);
			
					xSemaphoreTake(lcdLock, portMAX_DELAY);
					LCD_SetTextColor(White);
					LCD_SetBackColor(Black);
					LCD_DisplayChar(Line2, 100, Current[0]);
					LCD_DisplayChar(Line2, 80, Current[1]);
			    getCurrentHumidity(Current);
			    LCD_SetTextColor(0x7FFF);
					LCD_DisplayChar(Line4, 160, Current[0]);
					LCD_DisplayChar(Line4, 140, Current[1]);
					xSemaphoreGive(lcdLock);
			
				break;
			
			case SET_TEMPERATURE:
					getCurrentTemperature(Current);
			
					xSemaphoreTake(lcdLock, portMAX_DELAY);
					LCD_SetTextColor(White);
					LCD_SetBackColor(Black);
					LCD_DisplayChar(Line2, 180, Current[0]);
					LCD_DisplayChar(Line2, 160, Current[1]);
					LCD_DisplayChar(Line4, 180, Desired_Temp1);
					LCD_DisplayChar(Line4, 160, Desired_Temp2);
					xSemaphoreGive(lcdLock);
				break;
			
			case SETTINGS:
				break;
			
			case AUTHORS:
				break;	
		}			
    

		
		
		
		
    vTaskDelay(500 / portTICK_RATE_MS);
  }
}
/*-----------------------------------------------------------*/

/**
 * Register a callback that will be invoked when a touch screen
 * event occurs within a given rectangle
 *
 * NB: the callback function should have a short execution time,
 * since long-running callbacks will prevent further events from
 * being generated
 */

static TSCallback callbacks[64];
static u8 callbackNum = 0;

void registerTSCallback(u16 left, u16 right, u16 lower, u16 upper,
                        void (*callback)(u16 x, u16 y, u16 pressure, void *data),
						void *data, SCREEN_TypeDef myScreen) {
	callbacks[callbackNum].myScreen   = myScreen;						
  callbacks[callbackNum].lower    = lower;
  callbacks[callbackNum].upper    = upper;
  callbacks[callbackNum].left     = left;
  callbacks[callbackNum].right    = right;
  callbacks[callbackNum].callback = callback;
  callbacks[callbackNum].data     = data;
  callbackNum++;
}

static void touchScreenTask(void *params) {
  portTickType lastWakeTime = xTaskGetTickCount();
  TS_STATE *ts_state;
  u8 pressed = 0;
  u8 i;

  for (;;) {
    ts_state = IOE_TS_GetState();

	if (pressed) {
	  if (!ts_state->TouchDetected)
	    pressed = 0;
	} else if (ts_state->TouchDetected) {
	  for (i = 0; i < callbackNum; ++i) {
		if (callbacks[i].left   <= ts_state->X &&
		    callbacks[i].right  >= ts_state->X &&
		    callbacks[i].lower  >= ts_state->Y &&
		    callbacks[i].upper  <= ts_state->Y &&
		    callbacks[i].myScreen == Current_Screen)     // To check the menus
		  callbacks[i].callback(ts_state->X, ts_state->Y, ts_state->Z,callbacks[i].data);
	  }													
	  pressed = 1;
	}

//    if (ts_state->TouchDetected) {
//	  printf("%d,%d,%d ", ts_state->X, ts_state->Y, ts_state->Z);
//	}

	vTaskDelayUntil(&lastWakeTime, 100 / portTICK_RATE_MS);
  }
}

/*-----------------------------------------------------------*/

xQueueHandle buttonQueue;

static void highlightButton(u16 x, u16 y, u16 pressure, void *data) {
  u16 d = (int)data;
  xQueueSend(buttonQueue, &d, 0);
}

void setupButtons(void) {
  buttonQueue = xQueueCreate(4, sizeof(u16));
  
	/* Set temperature Screen Button */
	registerTSCallback(WIDTH - 300, WIDTH - 230, 210, 140, &highlightButton, (void*)SET_BUTTON, HOME);
	/* Fan screen Butoon */
	registerTSCallback(WIDTH - 200, WIDTH - 130, 210, 140, &highlightButton, (void*)FAN_BUTTON, HOME);
	/* Info Screen Button */
	registerTSCallback(WIDTH - 100, WIDTH - 30, 210, 140, &highlightButton, (void*)INFO_BUTTON, HOME);
	/* Home_button */
	registerTSCallback(WIDTH - 320, WIDTH - 280, 210, 170, &highlightButton, (void*)HOME_BUTTON, SET_TEMPERATURE);
	/* Home_button2 */
	registerTSCallback(WIDTH - 320, WIDTH - 280, 210, 170, &highlightButton, (void*)HOME_BUTTON2, SETTINGS);
	/* Button for returning from Info screen to Home screen (whole screen) */
	registerTSCallback(WIDTH - 320, WIDTH, 240, 0, &highlightButton, (void*)SCREEN_BUTTON, AUTHORS);
	/* Minus_button */
	registerTSCallback(WIDTH - 240, WIDTH - 200, 127, 87, &highlightButton, (void*)MINUS_BUTTON, SET_TEMPERATURE);
	/* Plus_button */
	registerTSCallback(WIDTH - 120, WIDTH - 80, 127, 87, &highlightButton, (void*)PLUS_BUTTON, SET_TEMPERATURE);
	/* Fan_switch_button */
	registerTSCallback(WIDTH - 220, WIDTH - 145, 77, 37, &highlightButton, (void*)AIR_SWITCH_BUTTON, SETTINGS);
	/* Resistor_switch_button */
	registerTSCallback(WIDTH - 220, WIDTH - 145, 127, 87, &highlightButton, (void*)RESISTOR_SWITCH_BUTTON, SETTINGS);
	/* Automatic_control_switch_button */
	registerTSCallback(WIDTH - 220, WIDTH - 145, 177, 137, &highlightButton, (void*)AUTO_SWITCH_BUTTON, SETTINGS);
	/* Confirm button */
	registerTSCallback(140, 180, 180, 140, &highlightButton, (void*)CONFIRM_BUTTON, SET_TEMPERATURE);
} 

BUTTONS_TypeDef buffer_b;

static void NavigateTask(void *params) {
	char temp[2];
	Events auto_on = AUTO_ON, auto_off = AUTO_OFF,
			   new_temp = NEW_TEMP, fan_on = FAN_ON,fan_off = FAN_OFF ,
				 heater_on = HEATER_ON, heater_off = HEATER_OFF;
	int state[3]={0,0,0};
	int i;
  for (;;) {
    xQueueReceive(buttonQueue, &buffer_b, portMAX_DELAY);
	  temp[0] = Desired_Temp1;
		temp[1] = Desired_Temp2;
		
		if (callbacks[buffer_b].myScreen == Current_Screen){
			xSemaphoreTake(lcdLock, portMAX_DELAY);
			
			switch (buffer_b){

				case SET_BUTTON:
					Current_Screen = SET_TEMPERATURE;
					Set_Temp_screen();
					break;
				
				case FAN_BUTTON:
					Current_Screen = SETTINGS;
					Fan_screen();
					for (i=0;i<3;i++)
					{ 
						if (state[i]==0)
						{
							Off_button(37+i*50,220);
						}
						else
						{
							On_button(37+i*50,220);
						}
					}
					break;
				
				case INFO_BUTTON:
				  Current_Screen = AUTHORS;
   				Info_screen();
					break;
				
				case HOME_BUTTON:
					Current_Screen = HOME;
				  Home_screen();
					break;
				
				/* From the settings menu*/
				case HOME_BUTTON2: 
				  Current_Screen = HOME;
					Home_screen();
					break;
				
				case SCREEN_BUTTON:
				  Current_Screen = HOME;
					Home_screen();
					break;
				
				case MINUS_BUTTON:
					if (temp[0] > '1'){
						temp[1]--;
						if (temp[1] < '0'){
							temp[1] = '9';
							temp[0]--;
						}
					}else{
						if (temp[1] > '6')
							temp[1]--;
					}
					break;
																				
				case PLUS_BUTTON:
				  if (temp[0] < '4'){
						temp[1]++;
						if (temp[1] > '9'){
							temp[1] = '0';
							temp[0]++;
						}
					}else{
						if (temp[1] < '5')
							temp[1]++;
					}
					break;
				case CONFIRM_BUTTON:
					//printf(" Glin! ");								
				  xQueueSend( EventQueue, &new_temp,  0); 
				//ToDo : Fix a value and then send it to the Micro
					break;
				
				case AIR_SWITCH_BUTTON:			
				if (state[0]==0 && state[2]==0){
					//printf(" Fan is now ON ");
					On_button(37,220);
					state[0]=1;											
				  xQueueSend( EventQueue, &fan_on,  0);
				}
				else if (state[0]!=0 && state[2]==0){
					//printf(" Fan is now OFF ");
					Off_button(37,220);
					state[0]=0;					
				  xQueueSend( EventQueue, &fan_off,  0);
				}
				else
					printf(" You have to disable Auto first");
				//ToDo : Fix a value and then send it to the Micro
					break;
				
				case RESISTOR_SWITCH_BUTTON:

				if (state[1]==0 && state[2]==0){
					//printf(" Resistor is now ON ");
					On_button(87,220);
					state[1]=1;										
				  xQueueSend( EventQueue, &heater_on,  0);
				}
				else if (state[1]!=0 && state[2]==0){
					//printf(" Resistor is now OFF ");
					Off_button(87,220);
					state[1]=0;											
				  xQueueSend( EventQueue, &heater_off,  0);
				}
				else
					printf(" You have to disable Auto first");
				//ToDo : Fix a value and then send it to the Micro
					break;
				
				case AUTO_SWITCH_BUTTON:
				if (state[2]==0){
					//printf(" Automatic control is now ON ");
					On_button(137,220);
					Off_button(37,220);
					Off_button(87,220);
					state[2]=1;
					state[0]=0;
					state[1]=0;											
				  xQueueSend( EventQueue, &auto_on,  0);
				}
				else{
					//printf(" Automatic control is now OFF ");
					Off_button(137,220);
					state[2]=0;								
				  xQueueSend( EventQueue, &auto_off,  0);
				}
				//ToDo : Fix a value and then send it to the Micro
					break;
			}
		}
		Desired_Temp1 = temp[0];
		Desired_Temp2 = temp[1];
    xSemaphoreGive(lcdLock);
  }
}

/*-----------------------------------------------------------*/


xQueueHandle EventQueue;

void initDisplay () {
  /* LCD Module init */
  lcdLock = xSemaphoreCreateMutex();  
	printQueue = xQueueCreate(128, 1);
	
	EventQueue = xQueueCreate(32, sizeof(Events));
	
	STM3210C_LCD_Init();
	Boot_screen();
	Home_screen();
	Current_Screen = HOME;

	xTaskCreate(DisplayTask, "DisplayTask", 100, NULL, 1, NULL);
	xTaskCreate(printTask, "print", 100, NULL, 1, NULL);
	xTaskCreate(touchScreenTask, "touchScreen", 100, NULL, 1, NULL);
  xTaskCreate(NavigateTask, "NavigateTask", 100, NULL, 1, NULL);
}
void getDesired(char *Array) {
   Array[0] = Desired_Temp1;
	 Array[1] = Desired_Temp2;
}
 /*-----------------------------------------------------------*/
/* Retarget printing to the serial port 1 */
int fputc(int ch, FILE *f) {
  unsigned char c = ch;
  xQueueSend(printQueue, &c, 0);
  return ch;
}
 /*-----------------------------------------------------------*/
