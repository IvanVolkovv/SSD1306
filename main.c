
#include <stdio.h>
#include <math.h>
#include "gpio_init.h"
#include "i2c_init.h"
#include "Driver_SSD1306.h" 
#include "Graphs_SSD1306.h" 
#include "Widgets_SSD1306.h"


/* --- prototypes --- */ 
void init_SysTickTMR(void); 
void delay(uint32_t); 

int main(void){
	
	// init_SysTickTMR();
	init_GPIO(); 
	init_I2C2(); 
	
	init_SSD1306();
	clearScreen_SSD1306();
	
	
	struct PropertiesGraphXY Graph = {0}; 
	initDefGraph(&Graph); 
	initGraph(&Graph); 
	createXYGraph(); 
		
	int32_t point = 0; 
	
	while(1){
		
		togglePin(GPIOC, GPIO_Pin_13);
		togglePin(GPIOB, GPIO_Pin_9);
		togglePin(GPIOB, GPIO_Pin_8);
		togglePin(GPIOB, GPIO_Pin_7);
		togglePin(GPIOB, GPIO_Pin_6);
	
		
		// setCoord(point, 10*sin(2*3.1415*0.05*point) + 10);

		//delay(1);			
		
		setCoord_D(point, 15*sin(2*3.1415*0.05*point) + 10 + 5*sin(2*3.1415*0.1*point) );
		++point;
		
		createXYGraph(); 		
	
		updateScreen_SSD1306();
		//delay(1);																						// 50 [ms]

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


