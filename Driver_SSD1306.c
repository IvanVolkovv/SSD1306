
#include "Driver_SSD1306.h"
#include "i2c_init.h"

// array to store the pixel field
uint8_t DisplayBuffer[GDDRAM_SIZE] = {0};

// Screen object
static SSD1306_t SSD1306 = {0, 0, 0, 0};


void 
init_SSD1306(void){
	
  uint8_t data[3];
	
  /* -------------------------------------------------------------------- */
	
  // Set display off // 0xAE экран выключен (sleep mode, режим сна. Состояние после сброса).
	// выключаю дисплей
  data[0] = 0xAE;
	sendCommand(I2C2, ADDR_SSD1306, data, 1); 
  
	/* -------------------------------------------------------------------- */ 
	
  // Set oscillator frequency
	// Set Display Clock Divide Ratio/Oscillator Frequency
  data[0] = 0xD5;
  data[1] = 0x80;
	sendCommand(I2C2, ADDR_SSD1306, data, 2);
  
	/* -------------------------------------------------------------------- */ 
	
  // Enable charge pump regulator
	// Таблица команд зарядового насоса
  data[0] = 0x8D;
  data[1] = 0x14;
  sendCommand(I2C2, ADDR_SSD1306, data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
  // Set display start line
  data[0] = 0x40;
  sendCommand(I2C2, ADDR_SSD1306, data, 1);
	
	/* -------------------------------------------------------------------- */ 
  
	// Set display offset
  data[0] = 0xD3;
  data[1] = 0;
  sendCommand(I2C2, ADDR_SSD1306, data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
	// Set COM output scan direction
  data[0] = 0xC0; /*data[0] = 0xC8;*/  
	sendCommand(I2C2, ADDR_SSD1306, data, 1);
	
	/* -------------------------------------------------------------------- */ 
	
	// Set MUX ratio
	data[0] = 0xA8;
	data[1] = 63; /*data[1] = 0x1F*/; 
  sendCommand(I2C2, ADDR_SSD1306, data, 2);
	
	/* -------------------------------------------------------------------- */ 
	
  // Set segment remap
	data[0] = 0xA0;   // data[0] = 0xA1; 
  sendCommand(I2C2, ADDR_SSD1306, data, 1);
  
	/* -------------------------------------------------------------------- */ 
  
  // Set COM pins hardware configuration
	data[0] = 0xDA;
	data[1] = 0x02; /*data[1] = 0x12*/;
	sendCommand(I2C2, ADDR_SSD1306, data, 2);
  
 
  
	/* ------------------------------------------------------------------- */ 
	
  // Set horizontal addressing mode
  data[0] = 0x20;
  data[1] = 0x00;
  sendCommand(I2C2, ADDR_SSD1306, data, 2);
  
  // Set column address
  data[0] = 0x21;
  data[1] = 0;
  data[2] = 127;
  sendCommand(I2C2, ADDR_SSD1306, data, 3);
  
  // Set page address
  data[0] = 0x22;
  data[1] = 0;
  data[2] = 3;
  sendCommand(I2C2, ADDR_SSD1306, data, 3);
  
  // Set contrast
	data[0] = 0x80;
	data[1] = 0x7F;
  sendCommand(I2C2, ADDR_SSD1306, data, 2);
	
	/* -------------------------------------------------------------------------------------------------------- */ 
	
	
  // Entire display on	// 0xA4, возобновить отображение содержимого RAM (состояние после сброса).
  data[0] = 0xA4;
  sendCommand(I2C2, ADDR_SSD1306, data, 1);
	
  //Set normal display	// 0xA6, нормальное отображение (состояние по умолчанию после сброса). Лог. 0 бита соответствует погашенной точке, 1 светящейся точке.
  data[0] = 0xA6;				// работает
	sendCommand(I2C2, ADDR_SSD1306, data, 1);
	
  // Set display on // 0xAF экран включен в нормальном режиме.
  data[0] = 0xAF;
	sendCommand(I2C2, ADDR_SSD1306, data, 1);
	
	
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
void drawPixel_SSD1306(uint8_t x, uint8_t y, SSD1306_COLOR color){
	
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

char ssd1306_WriteChar(char ch, SSD1306_Font_t Font, SSD1306_COLOR color) {
	
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
char ssd1306_WriteString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
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
void ssd1306_SetCursor(uint8_t x, uint8_t y) {
	SSD1306.CurrentX = x;
	SSD1306.CurrentY = y;
}






 







