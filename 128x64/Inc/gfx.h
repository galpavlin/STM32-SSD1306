#include "stm32f4xx_hal.h"


void draw_pixel(uint16_t y, uint16_t x, uint8_t color);
void invert_pixel(uint8_t x, uint8_t y);
void invert_rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void print_text(unsigned char left, unsigned char top, char *string);
void line_h(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t width, uint8_t mode);
void line_v(uint8_t y0, uint8_t y1, uint8_t x, uint8_t width, uint8_t mode);
void draw_rectangle(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t mode);
void draw_line( unsigned char x1, unsigned char y1, 
		unsigned char x2, unsigned char y2,
		unsigned char mode );
void graphics_text(unsigned char left, unsigned char top, unsigned char font, char *str);
void graphics_text_vertical(unsigned char left, unsigned char top, unsigned char font, char *str);

void draw_bitmap(uint8_t left, uint8_t top, const unsigned char *bitmap);
		
void rotate_point(float *x, float *y, float angle, float about_x, float about_y);
uint8_t start_animation(uint8_t whoscored);

void update_popup(void);
void set_popup(uint8_t displayedpopup, uint16_t duration);

void clear(void);
void draw_screen(void);

#define add 	0x01
#define erase 	0x00
#define invert 0x02
		

typedef struct twodimensions
{
	float x;
	float y;
} twodimensions_t;
		
typedef struct line
{
	twodimensions_t start;
	twodimensions_t stop;
} line_t;


enum popup
{
	store_settings = 0x00,
	undervolt = 0x01,
	tx_battery = 0x02
};
