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

#if defined ( SSD1306_128x32 ) && !defined ( SSD1306_128x64 ) && !defined ( SSD1306_128x128 )
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

#else 
#warning Display size not selected 
#endif
 

 /* --- list of commands --- */ 
#define DISPLAY_ON			0xAFu						// Set Display ON
#define DISPLAY_OFF			0xAEu						// Set Display OFF

#define DSPL_SET_CLK		0xD5u						// Set Display Clock
 
#define DSPL_SET_PUMP		0x8Du						// Charge Pump Setting
#define DSPL_PUMP_ON		0x14u						// Charge Pump ON
#define DSPL_PUMP_OFF		0x10u						// Charge Pump OFF

#define DSPL_STR_LINE		0x40u						// Set Display Start Line		

#define DSPL_SET_OFFSET	0xD3u						// Set Display Offset

// Set COM Output Scan Direction
#define DSPL_COM_SCAN_NORM	0xC0u				// normal mode		
#define DSPL_COM_SCAN_REMP	0xC8u				// remapped mode

#define DSPL_MTPLX_RATIO		0xA8u				// Set Multiplex Ratio

// Set Segment Re-map
#define DSPL_SGT_RMP_LEFT		0xA0u		
#define DSPL_SGT_RMP_RGHT		0xA1u	

// Set COM pins hardware configuration
#define DSPL_PIN_CNFG				0xDAu	
#define DSPL_PIN_SERIAL			0x00				// COM serial pin configuration.
#define DSPL_PIN_ALTRV			0x10u				// alternative COM pin configuration.
#define DSPL_PIN_RGHT				0x30u				// Enable COM Left/Right remap.
		

// Enumeration for screen colors
typedef enum {
	Black = 0x00, // Black color, no pixel
	White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR_t;

// Struct to store transformations
typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Initialized;
	uint8_t DisplayOn;
} SSD1306_t;


/* --- prototypes --- */ 
void setDisplayEnable(uint8_t); 
static void setDisplayClock(uint8_t, uint8_t);
static void setChargePump(uint8_t); 
static void setStartLine(uint8_t);
static void setDisplayOffset(uint8_t); 
static void setCOMScanDirection(uint8_t); 
static void setMultiplexRatio(uint8_t); 
static void setSegmentRemap(uint8_t);
static void setCOMHardConfig(uint8_t); 
void init_SSD1306(void);
void updateScreen_SSD1306(void); 
void clearScreen_SSD1306(void); 
void setPixel(uint8_t, uint8_t); 
void setSizeDrawArea(int32_t, int32_t, int32_t, int32_t); 
void setDefDrawArea(void); 

/* DEBUG */ 
void updateScreen_SSD1306_D(int32_t); 

/* import */ 
void drawPixel_SSD1306(uint8_t, uint8_t, SSD1306_COLOR_t);
static char ssd1306_WriteChar(char, SSD1306_Font_t, SSD1306_COLOR_t); 
char ssd1306_WriteString(char*, SSD1306_Font_t, SSD1306_COLOR_t);
void ssd1306_SetCursor(uint8_t, uint8_t); 


#endif /* __DRIVER_SSD1306_H */
