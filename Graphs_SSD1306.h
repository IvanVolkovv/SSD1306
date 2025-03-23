/*!
 * @file        Graphs_SSD1306.h // #include "Graphs_SSD1306.h"
 * @brief       for Grapg
 * @version     V1.0.0
 * @date       
 * @attention		- 
 *	
 * @Connection: 
*/

#ifndef __GRAPHS_SSD1306_H
#define __GRAPHS_SSD1306_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Driver_SSD1306.h"


#define X_MAX			(GDDRAM_SEG - 1)	// default max axis value, scale 1:1
#define Y_MAX			(GDDRAM_COM - 1)	// default max axis value, scale 1:1

#define DEFAULT_SCALE		0


typedef enum {
	XY_TYPE_1,												
	TYPE_2,												
	TYPE_3,												
	TYPE_4,												
} TypeGraph_SSD1306_t; 

typedef enum {
	NUM_Off,											
	NUM_On,												
} NumAxis;

struct PropertiesGraphXY{
	TypeGraph_SSD1306_t type;			// type of graph
	NumAxis numbers; 							// on/off numbers for axles
	int32_t xAxis_max; 						// maximum value of the X axis
	int32_t yAxis_max_min;				// maximum and minimum value of the Y axis
}; 

/* --- prototypes --- */ 
void createXYGraph(void); 
void initGraph(const struct PropertiesGraphXY*);
void initDefGraph(struct PropertiesGraphXY*); 
void setCoord(int32_t, int32_t);



#endif /* __GRAPHS_SSD1306_H */
