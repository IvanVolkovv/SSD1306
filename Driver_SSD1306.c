
#include "Driver_SSD1306.h"
#include "i2c_init.h"

/* declaration and definition ----------------------------------------- */ 
static uint8_t DisplayBuffer[GDDRAM_SIZE] = {0};
static struct SSD1306_t SSD1306 = {0, 0, 0, 0};


/*!
* @brief:	Function for enable display
* @param: enable - DISPLAY_ON or DISPLAY_OFF. 
*/
void
setDisplayEnable(uint8_t enable){
	if( enable == DISPLAY_ON || enable == DISPLAY_OFF )
		sendCommand(I2C2, ADDR_SSD1306, &enable, 1U); 	
}

/*!
* @brief:	Function for setting the clock frequency of the display
* @param: set_DCLK - Divide Ratio. Allowed values: 0 - 14 (the value is automatically increased by +1).  
*					set_F_OSC	- Oscillator Frequency. Allowed values: 0 - 15. 
*/
static void
setDisplayClock(uint8_t set_DCLK, uint8_t set_F_OSC){
	
	if( set_DCLK > 14U || set_F_OSC > 15U ){	
		set_DCLK = 0; 
		set_F_OSC = 0;	}
		
	set_DCLK |= (set_F_OSC << 4); 
	
	uint8_t DataSend[] = {DSPL_SET_CLK, set_DCLK}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for setting charge pump
* @param: set_pump - DSPL_PUMP_ON or DSPL_PUMP_OFF. 
*/
static void
setChargePump(uint8_t set_pump){
	
	if( set_pump != DSPL_PUMP_ON && set_pump != DSPL_PUMP_OFF )
		return; 
	
	uint8_t DataSend[] = {DSPL_SET_PUMP, set_pump}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set display start line
* @param: set_pump - DSPL_PUMP_ON or DSPL_PUMP_OFF. 
*/
static void
setStartLine(uint8_t number_line){
	
	if( number_line > 64 )
		return; 
	
	number_line |= DSPL_STR_LINE; 
	sendCommand(I2C2, ADDR_SSD1306, &number_line, 1U); 	
}

/*!
* @brief:	Function for set display offset
* @param: line_offset - Allowed values: 0 - 64
*/
static void
setDisplayOffset(uint8_t line_offset){
	
	if( line_offset >= 64 )
		return; 
	
	uint8_t DataSend[] = {DSPL_SET_OFFSET, line_offset}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set COM output scan direction
* @param: scan_direction - DSPL_COM_SCAN_NORM or DSPL_COM_SCAN_REMP. 
*/
static void
setCOMScanDirection(uint8_t scan_direction){
	
	if( scan_direction != DSPL_COM_SCAN_NORM && scan_direction != DSPL_COM_SCAN_REMP )
		return; 
	sendCommand(I2C2, ADDR_SSD1306, &scan_direction, 1U); 	
}

/*!
* @brief:	Function for set multiplex ratio
* @param: ratio - Allowed values: 15 - 63. 
*/
static void
setMultiplexRatio(uint8_t ratio){
	
	if( ratio < 15 || ratio >= 64 )
		return; 
	
	uint8_t DataSend[] = {DSPL_MTPLX_RATIO, ratio}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set segment re-map
* @param: remap - DSPL_SGT_RMP_LEFT or DSPL_SGT_RMP_RGHT. 
*/
static void
setSegmentRemap(uint8_t remap){
	
	if( remap != DSPL_SGT_RMP_LEFT && remap != DSPL_SGT_RMP_RGHT )
		return; 
	sendCommand(I2C2, ADDR_SSD1306, &remap, 1U); 	
}

/*!
* @brief:	Function for set COM pins hardware configuration
* @param: pins_config - DSPL_PIN_SERIAL, DSPL_PIN_ALTRV, DSPL_PIN_RGHT.  
*/
static void
setCOMHardConfig(uint8_t pins_config){
	
	if( pins_config != DSPL_PIN_SERIAL	&& 
			pins_config != DSPL_PIN_ALTRV 	&&
			pins_config != DSPL_PIN_RGHT		)
		return; 
	
	pins_config |= 0x02U; 
	
	uint8_t DataSend[] = {DSPL_PIN_CNFG, pins_config}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set memory addressing mode
* @param: mode - DSPL_SET_ADDR_HORZ, DSPL_SET_ADDR_VRTL, DSPL_SET_ADDR_PAGE.  
*/
static void
setMemAddrMode(uint8_t mode){
	
	if( mode > 2U )
		return; 
	
	uint8_t DataSend[] = {DSPL_SET_ADDR_MODE, mode}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set column address
* @param: addr_start - starting column addresses (allowed values: 0 - 127);
*					addr_end - final column addresses (allowed values: 0 - 127). 
*/
static void
setColumnAddr(uint8_t addr_start, uint8_t addr_end){
	
	if( addr_start > 127U || addr_end > 127U )
		return; 
	
	uint8_t DataSend[] = {DSPL_SET_COLM_ADDR, addr_start, addr_end}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set page address
* @param: addr_start - starting page addresses, (allowed values: 0 - GDDRAM_PAGES);
*					addr_end - final page addresses, (allowed values: 0 - GDDRAM_PAGES). 
*/
static void
setPageAddr(uint8_t addr_start, uint8_t addr_end){
	
	if( addr_start > GDDRAM_PAGES || addr_end > GDDRAM_PAGES )
		return; 
	
	uint8_t DataSend[] = {DSPL_SET_PAGE_ADDR, addr_start, addr_end}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for set contrast level
* @param: contrast - contrast level, (allowed values: 0 - 256). 
*/
static void
setContrastControl(uint8_t contrast){
	uint8_t DataSend[] = {DSPL_SET_CONTRAST, contrast}; 
	sendCommand(I2C2, ADDR_SSD1306, DataSend, sizeof(DataSend)); 	
}

/*!
* @brief:	Function for entire display
* @param: entire - entire display ON, ( allowed values: DSPL_SET_ENTR_RSM or DSPL_SET_ENTR_RSM ).  
*/
static void
setEntireEnable(uint8_t entire){
	if( entire != DSPL_SET_ENTR_RSM && entire != DSPL_SET_ENTR_FLL )
		return; 
	sendCommand(I2C2, ADDR_SSD1306, &entire, 1U);
}

/*!
* @brief:	Function for set normal/inverse display
* @param: inverse - normal or inverse mode, ( allowed values: DSPL_SET_INVR_OFF or DSPL_SET_INVR_ON ).  
*/
static void
setDisplayInverse(uint8_t inverse){
	if( inverse != DSPL_SET_INVR_OFF && inverse != DSPL_SET_INVR_ON )
		return; 
	sendCommand(I2C2, ADDR_SSD1306, &inverse, 1U);
}

/*!
* @brief:	Function for display initialization
*/
void 
init_SSD1306(void){
	
	setDisplayEnable(DISPLAY_OFF); 
	setDisplayClock(0x00, 0x08); 
	setChargePump(DSPL_PUMP_ON); 
	setStartLine(0);
	setDisplayOffset(0); 
	setCOMScanDirection(DSPL_COM_SCAN_NORM);
	setMultiplexRatio(63U); 
	setSegmentRemap(DSPL_SGT_RMP_LEFT); 
	setCOMHardConfig(DSPL_PIN_SERIAL); 
	setMemAddrMode(DSPL_SET_ADDR_HORZ); 
	setColumnAddr(0, GDDRAM_SEG - 1);
	setPageAddr(0, 3U); 
	setContrastControl(200); 
	setEntireEnable(DSPL_SET_ENTR_RSM); 
	setDisplayInverse(DSPL_SET_INVR_OFF); 
	setDisplayEnable(DISPLAY_ON); 
 
	// Set default values for screen object
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	SSD1306.Initialized = 1;
}

/*!
* @brief:	Function for update of screen
*/
void 
updateScreen_SSD1306(void){  
	sendData(I2C2, ADDR_SSD1306, DisplayBuffer, GDDRAM_SIZE);
}

/* DEBUG */ 
void 
updateScreen_SSD1306_D(int32_t size_buff){  
	sendData(I2C2, ADDR_SSD1306, DisplayBuffer, size_buff);
}

/*!
* @brief:	Function for clear screen
*/
void 
clearScreen_SSD1306(void){
  for (uint16_t i = 0; i < GDDRAM_SIZE; i++)
    DisplayBuffer[i] = 0x00;
  updateScreen_SSD1306();
}

/*!
* @brief:	Function to set a pixel to a coordinate
* @param: x, ó - coordinates; color - White or Black color of pixel. 
*/
void 
drawPixel_SSD1306(uint8_t x, uint8_t y, SSD1306_COLOR_t color){
	
	if( x >= GDDRAM_SEG || y >= GDDRAM_COM )
		return;															 															
   
	if( color == White )
		DisplayBuffer[x + (y / 8) * GDDRAM_SEG] |= 1 << (y % 8);
	else 
		DisplayBuffer[x + (y / 8) * GDDRAM_SEG] &= ~(1 << (y % 8));
}

/*!
* @brief:	Function for drawing a symbol
* @param: ch - symbol for drawing; 
*					Font - structure for storing font parameter; 
*					color - White or Black color of symbol. 
*/
static char 
ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR_t color) {
	
	uint32_t i, b, j;
    
	if( ch < 32 || ch > 126 )
		return 0;
	
	if( GDDRAM_SEG < (SSD1306.CurrentX + Font.width ) || GDDRAM_COM < (SSD1306.CurrentY + Font.height) )
		return 0;

	for(i = 0; i < Font.height; ++i){
		
		b = Font.data[(ch - 32) * Font.height + i];
		
		for(j = 0; j < Font.width; ++j){		
			
			if( (b << j) & 0x8000 )
				drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
			else
						drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);	
		
		}
	}
	
	SSD1306.CurrentX += Font.width;
	return ch;
}

/*!
* @brief:	Function for drawing a symbol
* @param: *str - pointer to string; 
*					Font - structure for storing font parameter; 
*					color - White or Black color of symbol. 
*/
char 
ssd1306_WriteString(char *str, SSD1306_Font_t Font, SSD1306_COLOR_t color) {

	while(*str){
		if( ssd1306_WriteChar(*str, Font, color) != *str )
			return *str;
		str++;
	}
 
	return *str;
}

/*!
* @brief:	Function to set the cursor position
* @param: x, ó - coordinates. 
*/
void 
ssd1306_SetCursor(uint8_t x, uint8_t y) {
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

/* DEBUG */ 
void 
setSizeDrawArea(int32_t start_col, int32_t end_col, int32_t start_page, int32_t end_page){
	
	uint8_t Data[3] = {0}; 
	
	// Set column address
  Data[0] = 0x21;
  Data[1] = start_col;
  Data[2] = end_col;
  sendCommand(I2C2, ADDR_SSD1306, Data, 3);
  
	// Set page address 
  Data[0] = 0x22;
  Data[1] = start_page;
  Data[2] = end_page;
  sendCommand(I2C2, ADDR_SSD1306, Data, 3);
	
}

/* DEBUG */ 
void 
setDefDrawArea(void){
	
	uint8_t Data[3] = {0}; 
	
	/* Set column address ---------------------------- */ 
  Data[0] = 0x21;
  Data[1] = 0;
  Data[2] = (GDDRAM_SEG - 1);
  sendCommand(I2C2, ADDR_SSD1306, Data, 3);
  
	/* Set page address ------------------------------ */ 
  Data[0] = 0x22;
  Data[1] = 0;
  Data[2] = (GDDRAM_COM >> 3U) - 1;
  sendCommand(I2C2, ADDR_SSD1306, Data, 3);
	
}

/*!
* @brief:	Function to shift display data array
* @param: shift - number of times to shift the array to the left.   
*/
void
shiftDisplayBuffer(uint8_t shift){	
	
	uint8_t quant_seg = GDDRAM_SEG - 1; 
	
	for(uint32_t sh = 0; sh < shift; ++sh){
	
		for(uint32_t i = 0; i < quant_seg; ++i){		
			DisplayBuffer[i] = DisplayBuffer[i+SFT_COM0_STOP]; 
			DisplayBuffer[i+SFT_COM1_STRT] = DisplayBuffer[i+SFT_COM1_STOP];		
			DisplayBuffer[i+SFT_COM2_STRT] = DisplayBuffer[i+SFT_COM2_STOP];		
			DisplayBuffer[i+SFT_COM3_STRT] = DisplayBuffer[i+SFT_COM3_STOP];		
		}
		DisplayBuffer[SFT_COM1_STRT-1] = 0x00;
		DisplayBuffer[SFT_COM2_STRT-1] = 0x00;
		DisplayBuffer[SFT_COM3_STRT-1] = 0x00;
		DisplayBuffer[GDDRAM_SIZE-1] = 0x00;
	}
		
}
