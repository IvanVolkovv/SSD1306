
#include <stdio.h>
#include <math.h>
#include "gpio_init.h"
#include "tim_init.h"
#include "i2c_init.h"
#include "Driver_SSD1306.h" 
#include "Graphs_SSD1306.h" 


int main(void){
	
	init_GPIO();
	init_SysTickTMR();
	init_I2C2(); 
	
	init_SSD1306();
	clearScreen_SSD1306();
	
	struct PropertiesGraphXY Graph = {
			.type = TYPE_1, 
			.numbers = NUM_On, 
			.xAxis_max = 10, 
			.yAxis_min = -10, 
			.yAxis_max = 10
	}; 

	initGraph(&Graph); 
	createXYGraph(); 
		
	int32_t point = 0; 
	
	while(1){
		
		setCoord(point, 15*sin(2*3.1415*0.05*point));  
		++point;

		if(point > 128){
			point = 0; 
			clearScreen_SSD1306();
			createXYGraph(); 
		}

		updateScreen_SSD1306();
				
		delay(100);																						// 100 [ms]
							
	}
			
	return 0;
	
}











