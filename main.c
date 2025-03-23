
#include <stdio.h>
#include <math.h>
#include "i2c_init.h"
#include "Driver_SSD1306.h" 
#include "Graphs_SSD1306.h" 

/* --- prototypes --- */ 
void init_SysTickTMR(void); 
void SysTick_Handler(void);
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

	//initGraph(&Graph); 
	createXYGraph(); 
		
	int32_t point = 0; 
	
	while(1){
		
		setCoord(point, 15*sin(2*3.1415*0.05*point));  
		++point;

		if(point > Graph.xAxis_max + 10){
			point = 0; 
			clearScreen_SSD1306();
			createXYGraph(); 
		}

		updateScreen_SSD1306();
				
		delay(50);																						// 50 [ms]
							
	}
			
	return 0;
	
}

void delay(uint32_t ms){
	
	uint32_t delay_1ms = (uint32_t)(SystemCoreClock / (1000 - 1)); 
	SysTick->LOAD = ms * delay_1ms; 																																																									
	SysTick->VAL = 0x00; 
	
	// включаю системный таймер
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;																													
	
	while( !( SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) )
		continue; 
	
	// выключаю системный таймер
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;																														
	
}


void init_SysTickTMR(void){

	SysTick->LOAD = 0x00; 																																							// Загрузка значения, от которого пойдет обратный отсчет																							
	SysTick->VAL = 0x00; 																																								// Обнуляем таймеры и флаги. Записью
	
	// таймер тактируется такой же частотой как и ядро МК
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 																												// частоту тактирования брать от тактирования процессора
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;																													// выключаю прерывания, когда таймер досчитает до 0
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;																														// выключаю системный таймер
}


