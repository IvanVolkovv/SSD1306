
#include "Widgets_SSD1306.h"

void 
createLoadLine(int32_t witch, int32_t height){
	/*
	
	*/
}

void 
setValueLoadLine(int32_t set_value){
	/*
	
	*/
}

int8_t 
drawSimpleLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
	
	if( checkCoordinate(x1, y1) || checkCoordinate(x2, y2) )
		return ERROR; 
	
	int32_t length_x = x2 - x1; 
	int32_t length_y = y2 - y1; 
	
	if( length_x < 0 || length_y < 0)
		return ERROR; 
	
	for(int32_t i = x1; i <= x2; ++i)
		drawPixel_SSD1306(i, y1, White); 
	for(int32_t i = y1; i <= y2; ++i)
		drawPixel_SSD1306(x1, i, White); 
	
	
	return DONE; 
	
}


/*!
* @brief:		The function checks that the coordinates of a point do not exceed the acceptable limits.
* @param: 	x_coord, y_coord - x and y coordinates of a point. 
* @retval:	ERROR or DONE. 
*/
int8_t 
checkCoordinate(int32_t x_coord, int32_t y_coord){
	
	if( x_coord < 0 || x_coord > GDDRAM_SEG)
		return ERROR; 
	if( y_coord < 0 || y_coord > GDDRAM_COM)
		return ERROR; 
	
	return DONE; 

}

int8_t 
createBattWidget(int32_t x_start, int32_t y_start, int32_t width, int32_t height){
	
	// drawSimpleLine(10, 5, 50, 5); 
	// drawSimpleLine(10, 5, 10, 25); 
	
	drawSimpleLine(x_start, y_start, x_start+width, y_start); 
	drawSimpleLine(x_start, y_start+height, x_start+width,  y_start+height); 
	drawSimpleLine(x_start, y_start, x_start, y_start+height); 
	drawSimpleLine(x_start+width, y_start, x_start+width,  y_start+height); 
	
}












