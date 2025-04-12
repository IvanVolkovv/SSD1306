
#include "Driver_SSD1306.h"
#include "i2c_init.h"

// array to store the pixel field
static uint8_t DisplayBuffer[GDDRAM_SIZE] = {0};

// Screen object
static SSD1306_t SSD1306 = {0, 0, 0, 0};


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
	
	if( number_line > 64)
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
	
	if( line_offset >= 64)
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
	
	if( ratio < 15 || ratio >= 64)
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
* @param: pins_config - DSPL_PIN_SERIAL or DSPL_PIN_ALTRV or DSPL_PIN_RGHT.  
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
* @brief:	Function for set COM pins hardware configuration
* @param: pins_config - DSPL_PIN_SERIAL or DSPL_PIN_ALTRV or DSPL_PIN_RGHT.  
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


void 
init_SSD1306(void){
	
  uint8_t Data[3];
	
	setDisplayEnable(DISPLAY_OFF); 
	setDisplayClock(0x00, 0x08); 
	setChargePump(DSPL_PUMP_ON); 
	setStartLine(0);
	setDisplayOffset(0); 
	setCOMScanDirection(DSPL_COM_SCAN_NORM);
	setMultiplexRatio(63U); 
	setSegmentRemap(DSPL_SGT_RMP_LEFT); 
	setCOMHardConfig(DSPL_PIN_SERIAL); 
  
	/* Set Memory Addressing Mode -------------------- */ 
	// Set horizontal addressing mode
  Data[0] = 0x20;
  Data[1] = 0x00;
  sendCommand(I2C2, ADDR_SSD1306, Data, 2);
  
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
  
	/* Set contrast ---------------------------------- */ 
	Data[0] = 0x80;
	Data[1] = 0x7F;
  sendCommand(I2C2, ADDR_SSD1306, Data, 2);
	
	/* -------------------------------------------------------------------------------------------------------- */ 
	
	
  // Entire display on	// 0xA4, возобновить отображение содержимого RAM (состояние после сброса).
  Data[0] = 0xA4;
  sendCommand(I2C2, ADDR_SSD1306, Data, 1);
	
  //Set normal display	// 0xA6, нормальное отображение (состояние по умолчанию после сброса). Лог. 0 бита соответствует погашенной точке, 1 светящейся точке.
  Data[0] = 0xA6;				// работает
	sendCommand(I2C2, ADDR_SSD1306, Data, 1);
	
	
	/* ------------------------------------------------------ */ 
	setDisplayEnable(DISPLAY_ON); 
	/* ------------------------------------------------------ */ 
	
	// Set default values for screen object
	SSD1306.CurrentX = 0;
	SSD1306.CurrentY = 0;
	SSD1306.Initialized = 1;
	
	/* ------------------------------------------------------ */ 
	
}

void 
updateScreen_SSD1306(void){  
	sendData(I2C2, ADDR_SSD1306, DisplayBuffer, GDDRAM_SIZE);
}

void 
updateScreen_SSD1306_D(int32_t size_buff){  
	sendData(I2C2, ADDR_SSD1306, DisplayBuffer, size_buff);
}

void 
clearScreen_SSD1306(void){
	
  for (uint16_t i = 0; i < GDDRAM_SIZE; i++)
    DisplayBuffer[i] = 0x00;
	
  updateScreen_SSD1306();
}

void 
setPixel(uint8_t x, uint8_t y){
  DisplayBuffer[x + (y / 8) * GDDRAM_SEG] |= (1 << (y % 8));
}

/*
 * Draw one pixel in the screenbuffer
 * X => X Coordinate
 * Y => Y Coordinate
 * color => Pixel color
 */
void 
drawPixel_SSD1306(uint8_t x, uint8_t y, SSD1306_COLOR_t color){
	
    if(x >= GDDRAM_SEG || y >= GDDRAM_COM)
        return;															 															// Don't write outside the buffer
   
    // Draw in the right color
    if(color == White)
        DisplayBuffer[x + (y / 8) * GDDRAM_SEG] |= 1 << (y % 8);
		else 
        DisplayBuffer[x + (y / 8) * GDDRAM_SEG] &= ~(1 << (y % 8));
}

/*
 * Draw 1 char to the screen buffer
 * ch       => char om weg te schrijven
 * Font     => Font waarmee we gaan schrijven
 * color    => Black or White
 */
static char 
ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR_t color) {
	
    uint32_t i, b, j;
    
    // Check if character is valid // Проверяем, является ли символ допустимым
    if (ch < 32 || ch > 126)
        return 0;
		
    // Проверяю остаётся ли еще место в текущей строке для записи символа
    if (GDDRAM_SEG < (SSD1306.CurrentX + Font.width) ||
        GDDRAM_COM < (SSD1306.CurrentY + Font.height))
    {
        // Недостаточно места в текущей строке
        return 0;
    }
    
    // Используйте шрифт для написания
    for(i = 0; i < Font.height; ++i){
			
				// нахожу первый байт символа в массиве data
				b = Font.data[(ch - 32) * Font.height + i];
			
				// накладываю маску, чтобы понять нужно высталять пиксель или нет
        for(j = 0; j < Font.width; ++j){
					
					if( (b << j) & 0x8000 )
						drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t) color);
					else
						drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR_t)!color);	
        }
    }
	
    // The current space is now taken
    SSD1306.CurrentX += Font.width;
    
    // Return written char for validation
    return ch;
}



/* Write full string to screenbuffer */
char 
ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR_t color) {
    while (*str) {
        if (ssd1306_WriteChar(*str, Font, color) != *str) {
            // Char could not be written
            return *str;
        }
        str++;
    }
    
    // Everything ok
    return *str;
}

/* Position the cursor */
void 
ssd1306_SetCursor(uint8_t x, uint8_t y) {
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}

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


















 







