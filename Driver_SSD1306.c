
#include "Driver_SSD1306.h"
#include "i2c_init.h"

// array to store the pixel field
uint8_t DisplayBuffer[GDDRAM_SIZE] = {0};

// Screen object
static SSD1306_t SSD1306 = {0, 0, 0, 0};


void 
init_SSD1306(void){
	
  uint8_t Data[3];
	
  /* -------------------------------------------------------------------- */
	
  // Set display off // 0xAE экран выключен (sleep mode, режим сна. Состояние после сброса).
	// выключаю дисплей
  Data[0] = 0xAE;
	sendCommand(I2C2, ADDR_SSD1306, Data, 1); 
  
	/* -------------------------------------------------------------------- */ 
	
  // Set oscillator frequency
	// Set Display Clock Divide Ratio/Oscillator Frequency
  Data[0] = 0xD5;
  Data[1] = 0x80;
	sendCommand(I2C2, ADDR_SSD1306, Data, 2);
  
	/* -------------------------------------------------------------------- */ 
	
  // Enable charge pump regulator
	// Таблица команд зарядового насоса
  Data[0] = 0x8D;
  Data[1] = 0x14;
  sendCommand(I2C2, ADDR_SSD1306, Data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
  // Set display start line
  Data[0] = 0x40;
  sendCommand(I2C2, ADDR_SSD1306, Data, 1);
	
	/* -------------------------------------------------------------------- */ 
  
	// Set display offset
  Data[0] = 0xD3;
  Data[1] = 0;
  sendCommand(I2C2, ADDR_SSD1306, Data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
	// Set COM output scan direction
  Data[0] = 0xC0; /*data[0] = 0xC8;*/  
	sendCommand(I2C2, ADDR_SSD1306, Data, 1);
	
	/* -------------------------------------------------------------------- */ 
	
	// Set MUX ratio
	Data[0] = 0xA8;
	Data[1] = 63; /*data[1] = 0x1F*/; 
  sendCommand(I2C2, ADDR_SSD1306, Data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
  // Set segment remap
	Data[0] = 0xA0;   // data[0] = 0xA1; 
  sendCommand(I2C2, ADDR_SSD1306, Data, 1);
  
	/* -------------------------------------------------------------------- */ 
  
  // Set COM pins hardware configuration
	Data[0] = 0xDA;
	Data[1] = 0x02; /*data[1] = 0x12*/;
	sendCommand(I2C2, ADDR_SSD1306, Data, 2);
  
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
	
  // Set display on // 0xAF экран включен в нормальном режиме.
  Data[0] = 0xAF;
	sendCommand(I2C2, ADDR_SSD1306, Data, 1);
	
	
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
drawPixel_SSD1306(uint8_t x, uint8_t y, SSD1306_COLOR color){
	
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
char 
ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
	
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
						drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR) color);
					else
						drawPixel_SSD1306(SSD1306.CurrentX + j, (SSD1306.CurrentY + i), (SSD1306_COLOR)!color);	
        }
    }
	
    // The current space is now taken
    SSD1306.CurrentX += Font.width;
    
    // Return written char for validation
    return ch;
}



/* Write full string to screenbuffer */
char 
ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
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


















 







