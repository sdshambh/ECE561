#ifndef LEDS_H
#define LEDS_H

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D

#define V_BLUE (3.0)
#define V_MAGENTA (2.6)
#define V_GREEN (2.2)
#define V_RED (1.7)

#define LED_PERIOD (50) // Measured in LPTMR interrupts ///opt1
#define LED_DURATION (1) // Measured in LPTMR interrupts ///opt1
//#define LED_PERIOD (5) // Measured in LPTMR interrupts 
//#define LED_DURATION (2) // Measured in LPTMR interrupts 


// function prototypes
void Init_RGB_LEDs(void);
void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
void Light_LEDs(float v);

#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
