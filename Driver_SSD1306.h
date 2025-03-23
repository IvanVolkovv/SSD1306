/*!
 * @file        Driver_SSD1306.h // #include "Driver_SSD1306.h"
 * @brief       API for SSD1306
 * @version     V1.0.0
 * @date        00.00.0000
 * @attention		- 
 *	
 * @Connection: 
 *
 *	–азмер диспле€ - 128*32 pixels (всего 4096 пикселей или 512 байт на весь дисплей)
 *	јдрес диспле€ (без смещени€) - 0x78
 *
*/

#ifndef __DRIVER_SSD1306_H
#define __DRIVER_SSD1306_H

#include "Fonts_SSD1306.h"

/* --- uncomment the line to select the display size --- */ 
#define SSD1306_128x32
// #define SSD1306_128x64
// #define SSD1306_128x128

/* --- device address --- */ 
#define ADDR_SSD1306					0x78

#if defined SSD1306_128x32 && !defined SSD1306_128x64 && !defined SSD1306_128x128
// for 128x32
#define GDDRAM_SEG						128								// X [pixels]
#define GDDRAM_COM     				32								// Y [pixels]	
#define GDDRAM_SIZE 					512								// [bytes]	(GDDRAM_SEG * GDDRAM_COM) / 8 	

#elif !defined SSD1306_128x32 && defined SSD1306_128x64 && !defined SSD1306_128x128
// for 128x64
#define GDDRAM_SEG						128							
#define GDDRAM_COM     				64							
#define GDDRAM_SIZE 					1024	

#elif !defined SSD1306_128x32 && !defined SSD1306_128x64 && defined SSD1306_128x128
// for 128x128
#define GDDRAM_SEG						128							
#define GDDRAM_COM     				128							
#define GDDRAM_SIZE 					2048

#endif

// Enumeration for screen colors
typedef enum {
	Black = 0x00, // Black color, no pixel
	White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

// Struct to store transformations
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Initialized;
	uint8_t DisplayOn;
} SSD1306_t;


/* --- prototypes --- */ 
void init_SSD1306(void);
void updateScreen_SSD1306(void); 
void clearScreen_SSD1306(void); 
void setPixel(uint8_t, uint8_t); 

/* import */ 
void drawPixel_SSD1306(uint8_t, uint8_t, SSD1306_COLOR);
char ssd1306_WriteChar(char, SSD1306_Font_t, SSD1306_COLOR); 
char ssd1306_WriteString(char*, SSD1306_Font_t, SSD1306_COLOR);
void ssd1306_SetCursor(uint8_t, uint8_t); 


#endif /* __DRIVER_SSD1306_H */
