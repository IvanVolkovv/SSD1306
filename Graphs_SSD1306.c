
#include "Graphs_SSD1306.h"

/* --- private variables --- */ 
struct PropertiesGraphXY PrivateGraph = {0}; 
float x_scale_factor = 1.; 
float y_scale_factor = 1.; 

/*!
* @brief:	Function to set graph properties
* @param:	*Graph - pointer to a data structure that contains custom properties. 
*/
void
initGraph(const struct PropertiesGraphXY* Graph){
	
	// copying the structure
	PrivateGraph.type = Graph->type; 
	PrivateGraph.numbers = Graph->numbers; 
	PrivateGraph.xAxis_max = Graph->xAxis_max; 
	PrivateGraph.yAxis_max_min = Graph->yAxis_max_min; 
	
	if( PrivateGraph.xAxis_max != X_MAX )
		x_scale_factor = X_MAX / (float)PrivateGraph.xAxis_max;	
	else
		x_scale_factor = 1.; 

	if( PrivateGraph.yAxis_max_min != Y_MAX ){
		
		if(Graph->type == XY_TYPE_1)
			y_scale_factor = (Y_MAX >> 1U /* /2 */) / (float)PrivateGraph.yAxis_max_min;
		else
			y_scale_factor = Y_MAX / (float)PrivateGraph.yAxis_max_min;
		
	}
	else{
		
		y_scale_factor = 1.; 
		
		if(PrivateGraph.type == XY_TYPE_1){
			PrivateGraph.yAxis_max_min = Y_MAX >> 1U; 
		}
		else if(PrivateGraph.type == TYPE_2){
			PrivateGraph.yAxis_max_min = Y_MAX; 
		}
	}
	
}

/*!
* @brief:	Function to write to default properties structure
* @param:	*Graph - pointer to a data structure that contains properties. 
*/
void
initDefGraph(struct PropertiesGraphXY* Graph){
	
	Graph->type = TYPE_2; 
	Graph->numbers = NUM_On; 
	Graph->xAxis_max = X_MAX;
	Graph->yAxis_max_min = 15;
	
}

/*!
* @brief:	Function to set the maximum value on the X axis
* @param: x_axis - maximum value. 
*/
void
setMaxAxisX(int32_t x_axis){
	
	if( x_axis <= 0 )
		return; 
	PrivateGraph.xAxis_max = x_axis; 
	
}

/*!
* @brief:	Function to set the maximum (and minimum) value for the Y axis
* @param: y_axis - maximum value. 
*/
void
setAxisY(int32_t y_axis){
	
	if( y_axis <= 0 )
		return;
	PrivateGraph.yAxis_max_min = y_axis; 
	
}

/*!
* @brief:	Function for enabling (disabling) axis numbering
* @param: num - takes on values: NUM_Off, NUM_On. 
*/
void
setAxisNumbers(NumAxis num){
	
	PrivateGraph.numbers = num; 
	
}

/*!
* @brief: Function draws a graph whose properties are set in initGraph()
*/
void 
createXYGraph(void){
	
	if(!CHECK_TYPE_GRAPH(PrivateGraph.type))
		return; 
	
	/* --- axis --- */ 
	
	// draw axis Y
	for(int j=0; j < GDDRAM_COM; ++j)
		drawPixel_SSD1306(0, j, White); 
	
	uint32_t axis_x = 0; 
	if( PrivateGraph.type == XY_TYPE_1 ){
		// draw axis X
		axis_x = GDDRAM_COM >> 1U;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White); 
	}
	else if( PrivateGraph.type == TYPE_2 ){
		// draw axis X
		axis_x = GDDRAM_COM - 1;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White);
	}
	
	/* --- numbers for the axis --- */ 
	
	if( PrivateGraph.numbers != 0 ){
			
		char BufForNumbers[10] = {0};
		// +Y
		ssd1306_SetCursor(2, 0);
		snprintf(BufForNumbers, sizeof BufForNumbers, "%d", PrivateGraph.yAxis_max_min);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// +X	
		int quant_chars = snprintf(BufForNumbers, sizeof BufForNumbers, "%d", PrivateGraph.xAxis_max);
		ssd1306_SetCursor(GDDRAM_SEG - quant_chars * 6, axis_x - 8);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// -Y
		if( PrivateGraph.type == XY_TYPE_1 ){
			ssd1306_SetCursor(2, GDDRAM_COM - 8);
			snprintf(BufForNumbers, sizeof BufForNumbers, "%d", -PrivateGraph.yAxis_max_min);
			ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		}
			
	}
	
}

/*!
* @brief: Function to clear the graph
*/
void
clearXYGraph(void){
	
	clearScreen_SSD1306();
	createXYGraph(); 
	
}

/*!
* @brief:	Function to set a point at X,Y coordinate
* @param: x_coord, y_coord - x and y coordinates of a point. 
*/
void 
setCoord(int32_t x_coord, int32_t y_coord){
	
	x_coord = (int32_t)(x_coord * x_scale_factor);
	y_coord = (int32_t)(y_coord * y_scale_factor);	
	
	if( (x_coord < 0) || (x_coord > X_MAX) )
		return; 
	
	if( PrivateGraph.type == XY_TYPE_1 ){
		
		int32_t y_base_line = Y_MAX >> 1U;  /* /2 */
		if( (y_coord < (-y_base_line)) || (y_coord > y_base_line) )
			return; 
	
		drawPixel_SSD1306(x_coord, y_base_line - y_coord, White); 
			
	}
	
	if( PrivateGraph.type == TYPE_2 ){
		
		int32_t y_base_line = Y_MAX; 
		if( (y_coord < 0) || (y_coord > y_base_line) )
			return; 
		
		drawPixel_SSD1306(x_coord, y_base_line - y_coord, White);
		
	}
	
}





 







