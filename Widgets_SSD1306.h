/*!
 * @file        Widgets_SSD1306.h // #include "Widgets_SSD1306.h"
 * @brief       for Widgets
 * @version     V1.0.0
 * @date        
 * @attention		- 
 *	
 * @Connection: 
 *
 *
*/

#ifndef __WIDGETS_SSD1306_H
#define __WIDGETS_SSD1306_H

#include "Driver_SSD1306.h"
#include "i2c_init.h"

#define DONE		0
#define ERROR 	(-1)


// Enumeration for battery widget size 
typedef enum {
	Size_8x4, 
	Size_12x6, 
	Size_16x8, 
	Size_20x10 
} BattWidgetSize;


/* --- prototypes --- */ 
void createLoadingLine(int32_t, int32_t);
void setValueLoadLine(int32_t); 

int8_t checkCoordinate(int32_t, int32_t); 
int8_t drawSimpleLine(int32_t, int32_t, int32_t, int32_t); 
int8_t createBattWidget(int32_t, int32_t, BattWidgetSize); 


#endif /* __WIDGETS_SSD1306_H */
