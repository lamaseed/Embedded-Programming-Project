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
	GLCD_clear(Black);							//Almost animation for booting!
	for (i=0; i<50; i++){
  GLCD_setTextColor(White);
	GLCD_setBackColor(Black);
	GLCD_displayStringLn(Line4, "      Group 5");}
		for (i=0; i<50; i++){
  GLCD_setTextColor(White);
	GLCD_setBackColor(Black);
	GLCD_displayStringLn(Line5, "    The Project");}
}

void Home_screen(void){
	GLCD_clear(Black);
	GLCD_setTextColor(Red);
  GLCD_fillRect(0, 0, 23, 319);
	GLCD_setBackColor(Red);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "        Home");
	GLCD_setBackColor(Black);
	GLCD_displayStringLn(Line2, "Temperature:");
	GLCD_displayStringLn(Line4, "Humidity:");
	Temp_button(175,265);
	Fan_button(175,165);
	Info_button(175,65);
}

void Temp_screen(void){
	GLCD_clear(Black);
	GLCD_setTextColor(Red);
  GLCD_fillRect(0, 0, 23, 319);
	GLCD_setBackColor(Red);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "   Set Temperature");
	GLCD_setBackColor(Black);
  GLCD_displayStringLn(Line2, "Current:");
	GLCD_displayStringLn(Line4, "Set:");
	Plus_button(87,80);
	Minus_button(87,200);
	Home_button(170,280);
}

void Fan_screen(void){
	GLCD_clear(Black);
	GLCD_setTextColor(Red);
  GLCD_fillRect(0, 0, 23, 319);
	GLCD_setBackColor(Red);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "      Settings");
	GLCD_setBackColor(Black);
	Home_button(170,280);
}

void Info_screen(void){
	GLCD_clear(Black);
	GLCD_setTextColor(Red);
  GLCD_fillRect(0, 0, 23, 319);
	GLCD_setBackColor(Red);
	GLCD_setTextColor(White);
	GLCD_displayStringLn(Line0, "       Authors");
	GLCD_setBackColor(Black);
	GLCD_displayStringLn(Line2, "- ZACHAROPOULOS");
	GLCD_displayStringLn(Line3, "- KARKANIS");
	GLCD_displayStringLn(Line4, "- NTOUNAS");
	GLCD_displayStringLn(Line5, "- STAMATAKOPOULOS");
	GLCD_displayStringLn(Line7, "Embedded Programming");
	GLCD_displayStringLn(Line8, "        2014");
	
}


/******* Create Circle given the coordinates of the center and the radius *********/
void DrawCircle(int x0, int y0, int radius)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
    GLCD_putPixel(x + x0, y + y0);
    GLCD_putPixel(y + x0, x + y0);
    GLCD_putPixel(-x + x0, y + y0);
    GLCD_putPixel(-y + x0, x + y0);
    GLCD_putPixel(-x + x0, -y + y0);
    GLCD_putPixel(-y + x0, -x + y0);
    GLCD_putPixel(x + x0, -y + y0);
    GLCD_putPixel(y + x0, -x + y0);
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

/********************* Create any line given two points ***********************/
void DrawAnyLine(int x0,int y0,int x1,int y1)
{
  int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = (dx>dy ? dx : -dy)/2, e2;
 
  for(;;){
    GLCD_putPixel(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = err;
    if (e2 >-dx) { err -= dy; x0 += sx; }
    if (e2 < dy) { err += dx; y0 += sy; }
  }
}

/******* Create Filled Circle given the coordinates of the center and the radius *********/

void DrawFilledCircle(int x0, int y0, int radius)
{
  int x = radius, y = 0;
  int radiusError = 1-x;
 
  while(x >= y)
  {
		DrawAnyLine(-x+x0,y+y0,x+x0,y+y0);
		DrawAnyLine(-y+x0,x+y0,y+x0,x+y0);
		DrawAnyLine(-x+x0,-y+y0,x+x0,-y+y0);
		DrawAnyLine(-y+x0,-x+y0,y+x0,-x+y0);
		
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
	
	GLCD_setTextColor(White);
	GLCD_drawRect(x-35, y-35, 70, 70);
	GLCD_setTextColor(Yellow);
	
	DrawFilledCircle(x, y, 10);
	
	DrawAnyLine(x,y-15,x,y-25);
	DrawAnyLine(x,y+15,x,y+25);
	DrawAnyLine(x-15,y,x-25,y);
	DrawAnyLine(x+15,y,x+25,y);
	
	DrawAnyLine(x-10,y-10,x-20,y-20);
	DrawAnyLine(x+10,y-10,x+20,y-20);
	DrawAnyLine(x-10,y+10,x-20,y+20);
	DrawAnyLine(x+10,y+10,x+20,y+20);

}

void Fan_button(int x, int y){
	
	//int x=115;
	//int y=165;
	GLCD_setTextColor(White);
	GLCD_drawRect(x-35, y-35, 70, 70);
	DrawFilledCircle(x, y, 25);
	GLCD_setTextColor(LightGrey );
	DrawFilledCircle(x, y, 5);
	GLCD_fillRect(x-10, y-3, 2, 6);
	GLCD_fillRect(x-14, y-4, 2, 8);
	GLCD_fillRect(x-18, y-5, 2, 10);
	GLCD_fillRect(x-22, y-6, 2, 12);
	
	GLCD_fillRect(x+8, y-3, 2, 6);
	GLCD_fillRect(x+12, y-4, 2, 8);
	GLCD_fillRect(x+16, y-5, 2, 10);
	GLCD_fillRect(x+20, y-6, 2, 12);
	
	GLCD_fillRect(x-3, y-10, 6, 2);
	GLCD_fillRect(x-4, y-14, 8, 2);
	GLCD_fillRect(x-5, y-18, 10, 2);
	GLCD_fillRect(x-6, y-22, 12, 2);
	
	GLCD_fillRect(x-3, y+8, 6, 2);
	GLCD_fillRect(x-4, y+12, 8, 2);
	GLCD_fillRect(x-5, y+16, 10, 2);
	GLCD_fillRect(x-6, y+20, 12, 2);
	
	GLCD_setTextColor(White);
	DrawFilledCircle(x, y, 2);

}


void Info_button(int x, int y){
	
	//int x=115;
	//int y=65;
	
	GLCD_setTextColor(White);
	GLCD_drawRect(x-35, y-35, 70, 70);
	
	GLCD_setTextColor(Blue);
	DrawFilledCircle(x, y, 25);
	
	GLCD_setTextColor(White);
	GLCD_fillRect(x-2, y-3, 20, 6);
	DrawFilledCircle(x-13, y, 5);

}

void Plus_button(int x, int y){
	
	GLCD_setTextColor(White);
	GLCD_drawRect(x, y, 40, 40);
	GLCD_fillRect(x+10, y+18, 20, 4);
	GLCD_fillRect(x+18, y+10, 4, 20);

}

void Minus_button(int x, int y){
	
	GLCD_setTextColor(White);
	GLCD_drawRect(x, y, 40, 40);
	GLCD_fillRect(x+18, y+10, 4, 20);


}

void Home_button(int x, int y){
	
	GLCD_setTextColor(White);
	GLCD_drawRect(x, y, 40, 40);
	GLCD_fillRect(x+22, y+12, 17, 16);
	GLCD_setTextColor(Red);
	DrawAnyLine(x+21,y+9,x+21,y+31);
	DrawAnyLine(x+20,y+10,x+20,y+30);
	DrawAnyLine(x+19,y+11,x+19,y+29);
	DrawAnyLine(x+18,y+12,x+18,y+28);
	DrawAnyLine(x+17,y+13,x+17,y+27);
	DrawAnyLine(x+16,y+14,x+16,y+26);
	DrawAnyLine(x+15,y+15,x+15,y+25);
	DrawAnyLine(x+14,y+16,x+14,y+24);
	DrawAnyLine(x+13,y+17,x+13,y+23);
	DrawAnyLine(x+12,y+18,x+12,y+22);
	DrawAnyLine(x+11,y+19,x+11,y+21);
	DrawAnyLine(x+10,y+20,x+10,y+20);
	GLCD_setTextColor(LightGrey);
	GLCD_fillRect(x+29, y+14, 10, 7);
	//GLCD_fillRect(x+21, y+11, 2, 18);

}



/*-----------------------------------------------------------*/
