
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
	
	while(1){
		
		setCoord(point, 15*sin(2*3.1415*0.05*point));  
		++point;

		if(point > Graph.xAxis_max + 10){
			point = 0;  
			clearXYGraph(); 
		}

		updateScreen_SSD1306();
				
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


