/*!
 * @file        Graphs_SSD1306.h // #include "Graphs_SSD1306.h"
 * @brief       The header file contains functions for drawing two-dimensional graphs on the X-Y surface.
 *	
*/

#ifndef __GRAPHS_SSD1306_H
#define __GRAPHS_SSD1306_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Driver_SSD1306.h"


#define X_MAX				(GDDRAM_SEG - 1)	// default max axis value, scale 1:1
#define Y_MAX				(GDDRAM_COM - 1)	// default max axis value, scale 1:1

#define CHECK_TYPE_GRAPH(TYPE)				(((TYPE) == XY_TYPE_1) || ((TYPE) == XY_TYPE_2))
																

typedef enum {
	XY_TYPE_1,												
	XY_TYPE_2,												
	TYPE_3,												
	TYPE_4											
} TypeGraph_SSD1306_t; 

typedef enum {
	NUM_Off,											
	NUM_On											
} NumAxis_t;

typedef enum {
	POINT,											
	STRAIGHT
} LineType_t;

typedef enum {
	SHIFT_no,											
	SHIFT_yes
} DataShift_t;

struct PropertiesGraphXY{
	TypeGraph_SSD1306_t type;					// type of graph
	NumAxis_t numbers; 								// on/off numbers for axles
	LineType_t line_type;  						// line type to draw
	int32_t xAxis_max; 								// maximum value of the X axis
	int32_t yAxis_max_min;						// maximum and minimum value of the Y axis
	DataShift_t data_shift; 					// enable or disable shifting of graph points
}; 

/* --- prototypes --- */ 
void createXYGraph(void); 
void initGraph(const struct PropertiesGraphXY*);
void initDefGraph(struct PropertiesGraphXY*); 
void setMaxAxisX(int32_t); 
void setAxisY(int32_t); 
void setAxisNumbers(NumAxis_t); 
void clearXYGraph(void); 
void updateGraph(void); 
void setCoord(int32_t, int32_t);


void setCoord_D(int32_t, int32_t); 





#endif /* __GRAPHS_SSD1306_H */
