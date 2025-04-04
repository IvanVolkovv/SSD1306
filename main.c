
#include <stdio.h>
#include <math.h>
#include "i2c_init.h"
#include "Driver_SSD1306.h" 
#include "Graphs_SSD1306.h" 

/* --- prototypes --- */ 
void init_SysTickTMR(void); 
void delay(uint32_t); 

int main(void){
	
	init_SysTickTMR();
	init_I2C2(); 
	
	init_SSD1306();
	clearScreen_SSD1306();
	
//	struct PropertiesGraphXY Graph = {
//			.type = XY_TYPE_1, 
//			.numbers = NUM_On, 
//			.xAxis_max = 50, 
//			.yAxis_max_min = 5
//	}; 
	
	struct PropertiesGraphXY Graph = {0}; 
	initDefGraph(&Graph); 
	initGraph(&Graph); 
	createXYGraph(); 
		
	int32_t point = 0; 
	
	setCoord(0, 20);  
	setCoord(10, 5); 
	setCoord(20, 10); 
	setCoord(30, 5);
	setCoord(40, 10);
	setCoord(50, 1);
	setCoord(60, 25);
	setCoord(70, 25);
	setCoord(80, 7);
	setCoord(90, 12);
	setCoord(100, 7);
	setCoord(120, 100); 
	updateGraph();
	
	while(1){
		
		/*
		setCoord(point, 30*sin(2*3.1415*0.02*point));  
		++point;

		if(point > Graph.xAxis_max + 10){
			point = 0;  
			clearXYGraph(); 
		}

		// updateScreen_SSD1306();
		updateGraph();
		*/		
		delay(50);																						// 50 [ms]
							
	}
			
	return 0;
	
}

/*!
* @brief:	Simple function for time delay
* @note:	Counter increment every 1ms
*/
void delay(uint32_t ms){
	
	uint32_t delay_1ms = (uint32_t)(SystemCoreClock / (1000 - 1)); 
	SysTick->LOAD = ms * delay_1ms; 																																																									
	SysTick->VAL = 0x00; 
	
	// enable system timer
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;																													
	
	while( !( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) )
		continue; 
	
	// turn off system timer
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;																														
	
}

/*!
* @brief:	Function for setting the system timer (SysTick)	
*/
void init_SysTickTMR(void){
	
	SysTick->LOAD = 0x00; 																																																														
	SysTick->VAL = 0x00; 																																								
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 																												
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;																													
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}


