#ifndef __SSD1306_FONTS_H__
#define __SSD1306_FONTS_H__

#include <stdint.h>
#include <stddef.h>


// Font 
typedef struct {
	const uint8_t width;                // Font width in pixels 
	const uint8_t height;               // Font height in pixels 
	const uint16_t *const data;         // Pointer to font data array 
	const uint8_t *const char_width;    // Proportional character width in pixels (NULL for monospaced) 
} SSD1306_Font_t;


/* --- declarations --- */ 
extern const SSD1306_Font_t Font_6x8;

#endif // __SSD1306_FONTS_H__
