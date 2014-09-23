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
 
#include "FreeRTOS.h"

#include "queue.h"
#include "stm32f10x.h"
#include "semphr.h"
#include "admin_delete.c"

#define WIDTH 320

typedef enum 
{ 
  SET_BUTTON 	= 0,
  FAN_BUTTON 	= 1,
  INFO_BUTTON = 2,
  HOME_BUTTON = 3,
	HOME_BUTTON2 = 4,
  SCREEN_BUTTON = 5,
  MINUS_BUTTON = 6,
  PLUS_BUTTON = 7,
	AIR_SWITCH_BUTTON = 8,
	RESISTOR_SWITCH_BUTTON = 9,
	AUTO_SWITCH_BUTTON =10,
	CONFIRM_BUTTON = 11
} BUTTONS_TypeDef;

typedef enum 
{ 
  HOME 	= 0,
  SET_TEMPERATURE = 1,
  SETTINGS = 2,
  AUTHORS = 3,

} SCREEN_TypeDef;

typedef struct {
  u16 lower, upper, left, right;
  SCREEN_TypeDef	myScreen;
  void *data;
  void (*callback)(u16 x, u16 y, u16 pressure, void *data);
} TSCallback;

void initDisplay (void);
void setupButtons(void);
void getDesired(char *Array);
	
extern void Boot_screen(void);
extern void DrawCircle(int x0, int y0, int radius);
extern void DrawFilledCircle(int x0, int y0, int radius);
extern void DrawAnyLine(int x0,int y0,int x1,int y1);
extern void Home_screen(void);
extern void Temp_button(int x, int y);
extern void Create_Fan_button(int x, int y);
extern void Create_Info_button(int x, int y);
extern void Create_Plus_button(int x, int y);
extern void Create_Minus_button(int x, int y);
extern void Create_Home_button(int x, int y);
extern void On_button(int x, int y);
extern void Off_button(int x, int y);
extern void Create_Confirm_button(int x, int y);
extern void Set_Temp_screen(void);
extern void Info_screen(void);
extern void Fan_screen(void);

