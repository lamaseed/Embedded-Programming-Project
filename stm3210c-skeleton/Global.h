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
 * This file defines datastructures used for communication between
 * the various modules
 */
#ifndef GLOBAL_H
#define GLOBAL_H


#include "FreeRTOS.h"
#include "stm32f10x.h"
#include "queue.h"

typedef enum {
	
	/* Deactivate automatic control*/
  AUTO_OFF = 0, AUTO_ON = 1,
  /* Activate automatic control*/
 
  /* Deactivate automatic control*/
  FAN_OFF = 2, FAN_ON = 3,
  /* Activate automatic control*/
	
  /* Deactivate automatic control*/	
  HEATER_OFF = 4, HEATER_ON = 5,	
	/* Activate automatic control*/

	NEW_TEMP = 6
	
} Events;

 /**
 * Queue on which events are propagated
 */
extern xQueueHandle EventQueue;
extern xQueueHandle TxQueue;

void getCurrentTemperature(char *Array);
void getCurrentHumidity(char *Array);
void getDesiredTemperature(char *Array);
#endif
