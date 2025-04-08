
#include "Widgets_SSD1306.h"

static int32_t 	x1_wdgt_batt = 0, 
								y1_wdgt_batt = 0, 
								x2_wdgt_batt = 0, 
								y2_wdgt_batt = 0,
								bat_sector_width = 0; 

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
createBattWidget(int32_t x_start, int32_t y_start, BattWidgetSize size_widget){
	
	int32_t width = 0, 
					height = 0,
					shift_cap = 0; 
	
	switch(size_widget){
		case Size_8x4:
			width = 8; height = 4; 
			shift_cap = 1; 
			break; 
		case Size_12x6:
			width = 12; height = 6;
			shift_cap = 2; 		
			break;
		case Size_16x8:
			width = 16; height = 8; 
			shift_cap = 2; 		
			break;	
		case Size_20x10:
			width = 20; height = 10; 
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
	
	/* 
	
		проверка, что с фигурой всё ок и тогда далее можно вычислять внутренний квадрат
		иначе нет
	*/ 
	
	x1_wdgt_batt = x_start + 1; 
	y1_wdgt_batt = y_start + 1; 
	x2_wdgt_batt = x_end - 1;  
	y2_wdgt_batt = y_end - 1;
	bat_sector_width = width / 4; 
	
}

int8_t
setBatLevel(uint8_t lvl){
	
	if( lvl < 0 || lvl > 4)
		return ERROR; 
	
	int32_t dividing_line = bat_sector_width * lvl;

	for(int8_t i = x1_wdgt_batt; i <= x2_wdgt_batt; ++i){
		
		if(dividing_line > 0){
			
		}
		else{
			
			
			
			
			
			
		}
		
		
		
		
	}
	
	
	
	
	
	
	
	
}










