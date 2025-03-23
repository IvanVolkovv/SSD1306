
#include "Graphs_SSD1306.h"

/* --- private variables --- */ 
struct PropertiesGraphXY *PrivateGraph = NULL; 
uint32_t x_max, y_max_min; 
float x_scale_factor = 0.; 
float y_scale_factor = 0.; 


void
initGraph(const struct PropertiesGraphXY* Graph){
	
	PrivateGraph = Graph; 
	x_max = PrivateGraph->xAxis_max;
	y_max_min = PrivateGraph->yAxis_max_min;
	
	if( x_max != DEFAULT_SCALE )
		x_scale_factor = X_MAX / (float)x_max;  
	else
		x_max = X_MAX; 
	
	if( y_max_min != DEFAULT_SCALE )
		y_scale_factor = (Y_MAX >> 1U /* /2 */) / (float)y_max_min;  
	else
		y_max_min = (Y_MAX >> 1U /* /2 */);	
	
}

void
initDefGraph(struct PropertiesGraphXY* Graph){
	Graph->type = XY_TYPE_1; 
	Graph->numbers = NUM_Off; 
	Graph->xAxis_max = X_MAX;
	Graph->yAxis_max_min = (Y_MAX >> 1U);
	initGraph(Graph); 
}

void
setMaxAxisX(int32_t x_axis){
	
	if(x_axis <= 0)
		return; 
	
	if(PrivateGraph != NULL)
		PrivateGraph->xAxis_max = x_axis; 
}

void 
createXYGraph(void){
	
	/* --- axis --- */ 
	
	// draw axis Y
	for(int j=0; j < GDDRAM_COM; ++j)
		drawPixel_SSD1306(0, j, White); 
	
	uint32_t axis_x = 0; 
	if( PrivateGraph->type == XY_TYPE_1 ){
		// draw axis X
		axis_x = GDDRAM_COM / 2;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White); 
	}
	else if( PrivateGraph->type == TYPE_2 ){
		// draw axis X
		axis_x = GDDRAM_COM - 1;
		for(int j=0; j < GDDRAM_SEG; ++j)
			drawPixel_SSD1306(j, axis_x, White);
	}
	
	/* --- numbers for the axis --- */ 
	
	if( PrivateGraph->numbers != 0 ){
			
		char BufForNumbers[10] = {0};
		// +Y
		ssd1306_SetCursor(2, 0);
		snprintf(BufForNumbers, sizeof BufForNumbers, "%d", y_max_min);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// +X	
		int quant_chars = snprintf(BufForNumbers, sizeof BufForNumbers, "%d", x_max);
		ssd1306_SetCursor(GDDRAM_SEG - quant_chars * 6, axis_x - 8);
		ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		// -Y
		if( PrivateGraph->type == XY_TYPE_1 ){
			ssd1306_SetCursor(2, GDDRAM_COM - 8);
			snprintf(BufForNumbers, sizeof BufForNumbers, "%d", -y_max_min);
			ssd1306_WriteString(BufForNumbers, Font_6x8, White);
		}
			
	}
	
}

void 
setCoord(int32_t x_coord, int32_t y_coord){
	
	if( x_max != DEFAULT_SCALE )
		x_coord = (int32_t)(x_coord * x_scale_factor);

	if( y_max_min != DEFAULT_SCALE )
		y_coord = (int32_t)(y_coord * y_scale_factor);	
	
	
	// check that the coordinates do not go beyond the permissible limits
	if( (x_coord < 0) || (x_coord > X_MAX) )
		return; 
	
	int32_t y_base_line = Y_MAX >> 1U;  /* /2 */
	if( (y_coord < (-y_base_line)) || (y_coord > y_base_line) )
		return; 
	
	
	if( PrivateGraph->type == XY_TYPE_1 ){
		
		drawPixel_SSD1306(x_coord, y_base_line - y_coord, White); 
			
	}
	
	if( PrivateGraph->type == TYPE_2 ){
		
		
		
		
	}
	
}



 







