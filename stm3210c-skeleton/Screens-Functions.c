/**
 * Program skeleton for the course "Programming embedded systems"
 *
 * Project Authors:
 * 	Zacharopoulos 		Georgios
 * 	Karkanis 					Iosif
 *	Ntounas						Georgios
 *	Stamatakopoulos		Panagiotis
 */


void Boot_screen(void) {
	int i;
	LCD_Clear(Black);							//Almost animation for booting!
	for (i=0; i<50; i++){
  LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line4, "      Group 5");}
		for (i=0; i<50; i++){
  LCD_SetTextColor(White);
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line5, "    The Project");}
		
	
}

void Home_screen(void){
	LCD_Clear(Black);
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line0, "        Home        ");
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line2, "Temperature:");
	LCD_DisplayStringLine(Line4, "Humidity:");
	Temp_button(175,265);
	Create_Fan_button(175,165);
	Create_Info_button(175,65);
}

void Set_Temp_screen(void){
	LCD_Clear(Black);
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line0, "   Set Temperature   ");
	LCD_SetBackColor(Black);
  LCD_DisplayStringLine(Line2, "Current:");
	LCD_DisplayStringLine(Line4, "Set:");
	Create_Confirm_button(140,180);
	Create_Plus_button(87,120);
	Create_Minus_button(87,240);
	Create_Home_button(170,280);
}

void Fan_screen(void){
	LCD_Clear(Black);
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line0, "      Settings      ");
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line2, "Air:");
	LCD_DisplayStringLine(Line4, "Heat:");
	LCD_DisplayStringLine(Line6, "Auto:");
//	On_button(37,220);
//	On_button(87,220);
//	On_button(137,220);
//	Off_button(37,150);
//	Off_button(87,150);
//	Off_button(137,150);
//		Off_button(37,220);
//		Off_button(87,220);
//		Off_button(137,220);
	Create_Home_button(170,280);

}

void Info_screen(void){
	LCD_Clear(Black);
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(Line0, "       Authors       ");
	LCD_SetBackColor(Black);
	LCD_DisplayStringLine(Line2, "- ZACHAROPOULOS");
	LCD_DisplayStringLine(Line3, "- KARKANIS");
	LCD_DisplayStringLine(Line4, "- NTOUNAS");
	LCD_DisplayStringLine(Line5, "- STAMATAKOPOULOS");
	LCD_DisplayStringLine(Line7, "Embedded Programming");
	LCD_DisplayStringLine(Line8, "        2014");
	
}


/******* Create Filled Circle given the coordinates of the center and the radius *********/

void DrawFilledCircle(int x0, int y0, int radius)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
		LCD_DrawUniLine(-x+x0,y+y0,x+x0,y+y0);
		LCD_DrawUniLine(-y+x0,x+y0,y+x0,x+y0);
		LCD_DrawUniLine(-x+x0,-y+y0,x+x0,-y+y0);
		LCD_DrawUniLine(-y+x0,-x+y0,y+x0,-x+y0);
		
    y++;
    if (radiusError<0)
    {
      radiusError += 2 * y + 1;
    } else {
      x--;
      radiusError+= 2 * (y - x + 1);
    }
  }
}

/******* Temperature button which is fixed and we just need to change x,y to change position *********/
void Temp_button(int x, int y){
	
	//int x=115;
	//int y=265;
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x-35, y+35, 70, 0x46);
	LCD_SetTextColor(Yellow);
	
	DrawFilledCircle(x, y, 10);
	
	LCD_DrawUniLine(x,y-15,x,y-25);
	LCD_DrawUniLine(x,y+15,x,y+25);
	LCD_DrawUniLine(x-15,y,x-25,y);
	LCD_DrawUniLine(x+15,y,x+25,y);
	
	LCD_DrawUniLine(x-10,y-10,x-20,y-20);
	LCD_DrawUniLine(x+10,y-10,x+20,y-20);
	LCD_DrawUniLine(x-10,y+10,x-20,y+20);
	LCD_DrawUniLine(x+10,y+10,x+20,y+20);

}

void Create_Fan_button(int x, int y){
	
	//int x=115;
	//int y=165;
	LCD_SetTextColor(White);
	LCD_DrawRect(x-35, y+35, 70, 70);
	DrawFilledCircle(x, y, 25);
	LCD_SetTextColor(LightGrey );
	DrawFilledCircle(x, y, 5);
	LCD_fillRect(x-10, y+3, 2, 6);
	LCD_fillRect(x-14, y+4, 2, 8);
	LCD_fillRect(x-18, y+5, 2, 10);
	LCD_fillRect(x-22, y+6, 2, 12);
	
	LCD_fillRect(x+8, y+3, 2, 6);
	LCD_fillRect(x+12, y+4, 2, 8);
	LCD_fillRect(x+16, y+5, 2, 10);
	LCD_fillRect(x+20, y+6, 2, 12);
	
	LCD_fillRect(x-3, y+10, 6, 2);
	LCD_fillRect(x-4, y+14, 8, 2);
	LCD_fillRect(x-5, y+18, 10, 2);
	LCD_fillRect(x-6, y+22, 12, 2);
	
	LCD_fillRect(x-3, y-8, 6, 2);
	LCD_fillRect(x-4, y-12, 8, 2);
	LCD_fillRect(x-5, y-16, 10, 2);
	LCD_fillRect(x-6, y-20, 12, 2);
	
	LCD_SetTextColor(White);
	DrawFilledCircle(x, y, 2);

}


void Create_Info_button(int x, int y){
	
	//int x=115;
	//int y=65;
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x-35, y+35, 70, 70);
	
	LCD_SetTextColor(Blue);
	DrawFilledCircle(x, y, 25);
	
	LCD_SetTextColor(White);
	LCD_fillRect(x-2, y+3, 20, 6);
	DrawFilledCircle(x-13, y, 5);
  
}
void Create_Confirm_button(int x, int y){
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y, 40, 40);
	LCD_SetTextColor(Green);
	LCD_DrawUniLine(x+20, y-1, x+40, y-15);
	LCD_DrawUniLine(x+19, y-1, x+40, y-16);
	LCD_DrawUniLine(x+18, y-1, x+40, y-17);
  LCD_DrawUniLine(x+40, y-15, x+1, y-38);
	LCD_DrawUniLine(x+39, y-15, x+1, y-37);
	LCD_DrawUniLine(x+38, y-15, x+1, y-36);
	
}
void Create_Plus_button(int x, int y){
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y, 40, 40);
	LCD_fillRect(x+10, y-18, 20, 4);
	LCD_fillRect(x+18, y-10, 4, 20);

}

void Create_Minus_button(int x, int y){
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y, 40, 40);
	LCD_fillRect(x+18, y-10, 4, 20);


}

void Create_Home_button(int x, int y){
	
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y+39, 40, 40);
	LCD_fillRect(x+22, y+28, 17, 16);
	LCD_SetTextColor(Red);
	LCD_DrawUniLine(x+21,y+9,x+21,y+31);
	LCD_DrawUniLine(x+20,y+10,x+20,y+30);
	LCD_DrawUniLine(x+19,y+11,x+19,y+29);
	LCD_DrawUniLine(x+18,y+12,x+18,y+28);
	LCD_DrawUniLine(x+17,y+13,x+17,y+27);
	LCD_DrawUniLine(x+16,y+14,x+16,y+26);
	LCD_DrawUniLine(x+15,y+15,x+15,y+25);
	LCD_DrawUniLine(x+14,y+16,x+14,y+24);
	LCD_DrawUniLine(x+13,y+17,x+13,y+23);
	LCD_DrawUniLine(x+12,y+18,x+12,y+22);
	LCD_DrawUniLine(x+11,y+19,x+11,y+21);
	LCD_DrawUniLine(x+10,y+20,x+10,y+20);
	LCD_SetTextColor(LightGrey);
	LCD_fillRect(x+29, y+26, 10, 7);
	//LCD_fillRect(x+21, y+11, 2, 18);

}


void On_button(int x, int y){
	LCD_SetTextColor(Green);
	LCD_fillRect(x, y, 40, 75);
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y, 40, 75);
	DrawFilledCircle(x+20, y-20, 10);
	LCD_fillRect(x+10, y-37, 20, 3);
	LCD_fillRect(x+10, y-53, 20, 3);
	LCD_DrawUniLine(x+10,y-40,x+27,y-53);
	LCD_DrawUniLine(x+11,y-40,x+28,y-53);
	LCD_DrawUniLine(x+12,y-40,x+29,y-53);
	LCD_DrawUniLine(x+13,y-40,x+30,y-53);
	LCD_SetTextColor(Green);
	DrawFilledCircle(x+20, y-20, 7);

}

//void NonActive_On_button(int x, int y){
//	LCD_SetTextColor(White);
//	LCD_DrawRect(x, y, 40, 60);
//	LCD_SetTextColor(LightGrey);
//	DrawFilledCircle(x+20, y-15, 10);
//	LCD_fillRect(x+10, y-32, 20, 3);
//	LCD_fillRect(x+10, y-47, 20, 3);
//	LCD_DrawUniLine(x+10,y-35,x+27,y-47);
//	LCD_DrawUniLine(x+11,y-35,x+28,y-47);
//	LCD_DrawUniLine(x+12,y-35,x+29,y-47);
//	LCD_DrawUniLine(x+13,y-35,x+30,y-47);
//	LCD_SetTextColor(Black);
//	DrawFilledCircle(x+20, y-15, 7);

//}

void Off_button(int x, int y){
	LCD_SetTextColor(Red);
	LCD_fillRect(x, y, 40, 75);
	LCD_SetTextColor(White);
	LCD_DrawRect(x, y, 40, 75);
	DrawFilledCircle(x+20, y-15, 10);
	LCD_fillRect(x+10, y-32, 20, 2);
	LCD_fillRect(x+10, y-35, 2, 10);
	LCD_fillRect(x+19, y-35, 2, 8);
	LCD_fillRect(x+10, y-50, 20, 2);
	LCD_fillRect(x+10, y-53, 2, 10);
	LCD_fillRect(x+19, y-53, 2, 8);
	LCD_SetTextColor(Red);
	DrawFilledCircle(x+20, y-15, 7);
	
}
//void NonActive_Off_button(int x, int y){
//	LCD_SetTextColor(White);
//	LCD_DrawRect(x, y, 40, 75);
//	LCD_SetTextColor(LightGrey);
//	DrawFilledCircle(x+20, y-15, 10);
//	LCD_fillRect(x+10, y-32, 20, 2);
//	LCD_fillRect(x+10, y-35, 2, 10);
//	LCD_fillRect(x+19, y-35, 2, 8);
//	LCD_fillRect(x+10, y-50, 20, 2);
//	LCD_fillRect(x+10, y-53, 2, 10);
//	LCD_fillRect(x+19, y-53, 2, 8);
//	LCD_SetTextColor(Black);
//	DrawFilledCircle(x+20, y-15, 7);
//	
//}
/*-----------------------------------------------------------*/
