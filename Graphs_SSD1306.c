
#include "Graphs_SSD1306.h"

/* --- private variables --- */ 
static TypeGraph_SSD1306_t type;							
static NumAxis num; 													
static int32_t xAxis_max; 										
static int32_t yAxis_max;											
static int32_t yAxis_min; 									

void
initGraph(const struct PropertiesGraphXY* Graph){
	type = Graph->type; 
	num = Graph->numbers; 
	xAxis_max = Graph->xAxis_max; 
	yAxis_max = Graph->yAxis_max; 
	yAxis_min = Graph->yAxis_min; 
	
}

void 
createXYGraph(void){
	
	/* --- axis --- */ 
	
	// draw axis Y
	for(int j=0; j < GDDRAM_COM; ++j)
		drawPixel_SSD1306(0, j, White); 
	
	uint32_t axis_x = 0; 
	if( type == TYPE_1 ){
		// draw axis X
		axis_x = GDDRAM_COM / 2;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White); 
	}
	else if( type == TYPE_2 ){
		// draw axis X
		axis_x = GDDRAM_COM - 1;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White);
	}
	
	/* --- numbers for the axis --- */ 
	
	if( num != 0 ){
			
		char BufForNumbers[5] = {0};
		// +Y
		ssd1306_SetCursor(2, 0);
		snprintf(BufForNumbers, sizeof BufForNumbers, "%d", 1);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// +X	
		int quant_chars = snprintf(BufForNumbers, sizeof BufForNumbers, "%d", 12);
		ssd1306_SetCursor(GDDRAM_SEG - quant_chars * 6, axis_x - 8);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// -Y
		if( type == TYPE_1 ){
			ssd1306_SetCursor(2, GDDRAM_COM - 8);
			ssd1306_WriteChar((char)'-', Font_6x8, White); 
			ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		}
			
	}
	
}

void 
setCoord(int32_t x_coord, int32_t y_coord){
	
	// check that the coordinates do not go beyond the permissible limits
	if( (x_coord < 0) || (x_coord > 128) )
		return; 
	if( (y_coord < (-16)) || (y_coord > 16) )
		return; 
	
	/* 
	
		need to calculate ranges
	
	*/ 
	
	
	if( type == TYPE_1 ){
		
		int line_y = GDDRAM_COM / 2;
		
		drawPixel_SSD1306(x_coord, line_y - y_coord, White); 
			
	}
	
	if( type == TYPE_2 ){
		
		
		
		
	}
	
}



 







