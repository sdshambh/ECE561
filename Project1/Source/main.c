/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"

#include "LCD.h"
#include "font.h"

#include "LEDs.h"
#include "timers.h"
#include "UART.h"

#include "JPEG_image.h"

#include "I2C.h"
#include "mma8451.h"
#include "delay.h"
#include "profile.h"
#include "math.h"

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
//	uint16_t r;
//	char buffer[32];
	uint32_t hash;
	PT_T p;
	
	Init_RGB_LEDs();

	LCD_Init();
	LCD_Text_Init(1);
	LCD_Erase();
	
	p.X = COL_TO_X(0);
	p.Y = ROW_TO_Y(0);
	LCD_Text_PrintStr(&p, "Initializing");
	
	Init_Profiling();
	LCD_Erase();
	Control_RGB_LEDs(1,0,1); // Magenta: running
	Enable_Profiling();
	hash = LCD_JPEG();
	Disable_Profiling();
	p.X = COL_TO_X(1);
	p.Y = ROW_TO_Y(23);

#if ENABLE_PIXEL_HASH
	if (hash != CORRECT_IMAGE_HASH) {
		Control_RGB_LEDs(1,0,0);	// Red: done with error
		LCD_Text_Set_Colors((COLOR_T*)&C_BLACK, (COLOR_T*)&C_RED);
		LCD_Text_PrintStr(&p, "****** Bad image hash! ******");
		while (1)
			;
	} else {
		Control_RGB_LEDs(0,0,1);	// Blue: done correctly
		LCD_Text_Set_Colors((COLOR_T*)&C_BLACK, (COLOR_T*)&C_GREEN);
		LCD_Text_PrintStr(&p, "****** Good image hash ******");
		Delay(1000);
	}
#endif
	
	LCD_Text_Set_Colors((COLOR_T*)&C_WHITE, (COLOR_T*)&C_BLACK);
	Sort_Profile_Regions(); 
	LCD_Erase();
	Display_Profile();
	
	while (1)
		;
		
}

