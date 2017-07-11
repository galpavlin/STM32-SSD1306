#include "stm32f0xx_hal.h"

void draw_pixel(uint8_t x, uint8_t y, uint8_t color);
void invert_pixel(uint8_t x, uint8_t y);
void invert_rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void print_text(unsigned char left, unsigned char top, char *string);
void line_h(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t width, uint8_t mode);
void line_v(uint8_t y0, uint8_t y1, uint8_t x, uint8_t width, uint8_t mode);
void graphics_text(unsigned char left, unsigned char top, unsigned char font, char *str);


void draw_bitmap(uint8_t left, uint8_t top, const unsigned char *bitmap);

void clear(void);

#define add 	0x01
#define erase 	0x00
#define invert 0x02

