
#include <stdio.h>
#include <math.h>
#include "i2c_init.h"
#include "Driver_SSD1306.h" 
#include "Graphs_SSD1306.h" 
#include "Widgets_SSD1306.h"


/* --- prototypes --- */ 
void init_SysTickTMR(void); 
void delay(uint32_t); 

int main(void){
	
	init_SysTickTMR();
	init_I2C2(); 
	
	init_SSD1306();
	clearScreen_SSD1306();

/* ------------------------------------------------------------------------ */ 	
	
//	struct PropertiesGraphXY Graph = {
//			.type = XY_TYPE_1, 
//			.numbers = NUM_On, 
//			.xAxis_max = 50, 
//			.yAxis_max_min = 5
//	}; 
	
//	struct PropertiesGraphXY Graph = {0}; 
//	initDefGraph(&Graph); 
//	initGraph(&Graph); 
//	createXYGraph(); 
//		
//	int32_t point = 0; 
//	
//	setCoord(0, 20);  
//	setCoord(10, 5); 
//	setCoord(20, 10); 
//	setCoord(30, 5);
//	setCoord(40, 10);
//	setCoord(50, 1);
//	setCoord(60, 25);
//	setCoord(70, 25);
//	setCoord(80, 7);
//	setCoord(90, 12);
//	setCoord(100, 7);
//	setCoord(120, 100); 
//	updateGraph();
	
	
	
	// setSizeDrawArea(50, 60, 0, 1); 
	
	/*
	drawPixel_SSD1306(0, 0, White); 
	drawPixel_SSD1306(1, 1, White); 
	drawPixel_SSD1306(2, 2, White);
	drawPixel_SSD1306(50, 8, White); 
	drawPixel_SSD1306(51, 9, White); 
	drawPixel_SSD1306(52, 10, White); 
	 */ 
	
	
	/*
	for(int i=0; i < 10; ++i)
		for(int j=0; j < 8; ++j)
			drawPixel_SSD1306(i, j, White); 
	
	// updateScreen_SSD1306();   
	updateScreen_SSD1306_D(10); 
	*/ 
	
/* ---------------------------------------------------------------------------- */ 


	// drawSimpleLine(10, 5, 50, 5); 
	// drawSimpleLine(10, 5, 10, 25); 
	
	// 8õ4
	// 12õ6 **
	// 16õ8 **
	// 20õ10 ** 
	createBattWidget(50, 5, Size_20x10);
	setBatLevel(0); 	
	updateScreen_SSD1306();

	
	int32_t lvl = 0; 
	
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
		
		
		setBatLevel(lvl); 	
		updateScreen_SSD1306();
		
		
		
		
		if(lvl == 4)
			lvl = 0; 
		else
			++lvl;
		
		delay(1000);																						// 50 [ms]

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
	//SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 																												
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;																													
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	
}


