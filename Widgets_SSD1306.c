
#include "Widgets_SSD1306.h"

static struct BatWdtProperties BatWdtProp = {0}; 


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

/*!
* @brief:		The function draws a simple horizontal or vertical line.
* @param: 	x1, y1 - x and y coordinates of the beginning of the segment, 
						x2, y2 - x and y coordinates of the end of the segment. 
* @retval:	ERROR or DONE. 
*/
int8_t 
drawSimpleLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
	
	if( checkCoordinate(x1, y1) || checkCoordinate(x2, y2) )
		return ERROR; 

	if( x2 - x1 < 0 || y2 - y1 < 0)
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

/*!
* @brief:		The function draws battery widget
* @param: 	x_start, y_start - x and y coordinates of the upper left corner of the widget; 
*						size_widget - widget size. 
* @retval:	ERROR or DONE. 
*/
int8_t 
createBattWidget(int32_t x_start, int32_t y_start, BattWidgetSize size_widget){
	
	int32_t width = 0, 
					height = 0,
					shift_cap = 0; 
	
	switch(size_widget){
		case Size_8x4:
			width = 10; height = 6; 
			shift_cap = 1; 
			break; 
		case Size_12x6:
			width = 14; height = 8;
			shift_cap = 2; 		
			break;
		case Size_16x8:
			width = 18; height = 10; 
			shift_cap = 2; 		
			break;	
		case Size_20x10:
			width = 22; height = 12; 
			shift_cap = 2; 		
			break;
		default:
			return ERROR; 
			break; 
	}
	
	int32_t x_end = x_start + width; 
	int32_t y_end = y_start + height; 
	
	drawSimpleLine(x_start, y_start, x_end, y_start); 
	drawSimpleLine(x_start, y_end, x_end,  y_end); 
	drawSimpleLine(x_start, y_start, x_start, y_end); 
	drawSimpleLine(x_end, y_start, x_end,  y_end); 
	drawSimpleLine(x_end+1, y_start+shift_cap, x_end+1, y_end-shift_cap);
	drawSimpleLine(x_end+2, y_start+shift_cap, x_end+2, y_end-shift_cap);	
	
	/* проверка, что с фигурой всё ок и тогда далее можно вычислять внутренний квадрат иначе нет */ 
	
	BatWdtProp.x1_inner_rect = x_start + 1;
	BatWdtProp.y1_inner_rect = y_start + 1; 
	BatWdtProp.x2_inner_rect = x_end - 1; 
	BatWdtProp.y2_inner_rect = y_end - 1;
	BatWdtProp.segment_width = (width-2) / 4; 
	
}

/*!
* @brief:		Set the charge level value
* @param: 	lvl - Charge level, takes a value from 0 to 4. 
						One filled widget segment means 25% charge. As a result, 4 * 25% = 100%.
* @retval:	ERROR or DONE. 
*/
int8_t
setBatLevel(uint8_t lvl){
	
	if( lvl < 0 || lvl > 4)
		return ERROR; 
	
	int32_t dividing_line = BatWdtProp.segment_width * lvl;

	uint8_t color_line = 0; 
	for(int8_t i = BatWdtProp.x1_inner_rect; i <= BatWdtProp.x2_inner_rect; ++i){
		color_line = (dividing_line-- >= 0); 
		for(int8_t j = BatWdtProp.y1_inner_rect; j <= BatWdtProp.y2_inner_rect; ++j)
			drawPixel_SSD1306(i, j, color_line);
	}
}

