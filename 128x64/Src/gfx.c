#include "stm32f4xx_hal.h"
#include "gfx.h"
#include "radar.h"
#include <stdio.h>
#include "fonts.h"
#include <math.h>
#include "fft.h"
#include "arm_math.h"
#include "string.h"
#include "buttons.h"

/*
COORDINATES for 128*32 pixels (x,y)
|----------------------------|
|0,0							127,0|
|0,31						  127,31|
|----------------------------|
*/

/*
COORDINATES for 128*64 pixels (x,y)
|----------------------------|
|0,0							127,0|
|0,63						  127,63|
|----------------------------|
*/



uint8_t gddram[8][128];
extern float32_t Bars[FFT_SIZE];


void draw_pixel(uint16_t y, uint16_t x, uint8_t color)
{
	if(x<64 && y<128)
	{
		//x=64-x;
		//y=127-y;
		uint8_t pixel = 0x01;
		uint8_t line = x>>3;
		uint8_t byte = pixel<<(x%8);
		if(color)
			gddram[line][y] |= byte;
		else
			gddram[line][y] &= ~byte;
	}
}

void draw_pixel_triple(uint8_t x, uint8_t y, uint8_t color)
{
	uint8_t i,j;
	if(x<128/3 && y<64/3)
	{
		for(i=0;i<3;i++)
			for(j=0;j<3;j++)
				draw_pixel(x*3+i,y*3+j,color);
	}
}

void invert_pixel(uint8_t x, uint8_t y)
{
	if(x<128 && y<64)
	{
		uint8_t pixel = 0x01;
		gddram[y>>3][x] ^= pixel<<(y%8);
	}
}

void invert_rectangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t i,j;
	for(i=x0;i<x1;i++)
		for(j=y0;j<y1;j++)
			invert_pixel(i,j);
}

void clear(void)
{
	uint8_t i,j;
	for(i=0;i<8;i++)
		for(j=0;j<128;j++)
			gddram[i][j]=0x00;
}

void line_v(uint8_t y0, uint8_t y1, uint8_t x, uint8_t width, uint8_t mode)
{
	if(y0>y1)
	{
		uint8_t temp = y0;
		y0 = y1;
		y1 = temp;
	}
	uint8_t i,j;
	switch(mode)
	{
		case add : 
			if(width%2)
			{
				uint8_t start = x-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						draw_pixel(start+i,j,1);
			}
			else
			{
				uint8_t start = x-width/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						draw_pixel(start+i,j,1);
			}
			break;
			
		case erase : 
			if(width%2)
			{
				uint8_t start = x-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						draw_pixel(start+i,j,0);
			}
			else
			{
				uint8_t start = x-width/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						draw_pixel(start+i,j,0);
			}
			break;
			
		case invert : 
			if(width%2)
			{
				uint8_t start = x-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						invert_pixel(start+i,j);
			}
			else
			{
				uint8_t start = x-width/2;
				for(i=0;i<width;i++)
					for(j=y0;j<y1;j++)
						invert_pixel(start+i,j);
			}
			break;
	}
}

void line_h(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t width, uint8_t mode)
{
	if(x0>x1)
	{
		uint8_t temp = x0;
		x0 = x1;
		x1 = temp;
	}
	uint8_t i,j;
	switch(mode)
	{
		case add : 
			if(width%2)
			{
				uint8_t start = y0-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						draw_pixel(j,start+i,1);
			}
			else
			{
				uint8_t start = y0-width/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						draw_pixel(j,start+i,1);
			}
			break;
			
		case erase : 
			if(width%2)
			{
				uint8_t start = y0-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						draw_pixel(j,start+i,0);
			}
			else
			{
				uint8_t start = y0-width/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						draw_pixel(j,start+i,0);
			}
			break;
			
		case invert : 
			if(width%2)
			{
				uint8_t start = y0-(width-1)/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						invert_pixel(j,start+i);
			}
			else
			{
				uint8_t start = y0-width/2;
				for(i=0;i<width;i++)
					for(j=x0;j<=x1;j++)
						invert_pixel(j,start+i);
			}
			break;

	}
}

void draw_rectangle(uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint8_t mode)
{
	uint8_t i,j,buffer;
	if(x1>x2)
	{
		buffer = x2;
		x2 = x1;
		x1 = buffer;
	}
	
	if(y1>y2)
	{
		buffer = y2;
		y2 = y1;
		y1 = buffer;
	}
	for(i=x1;i<=x2;i++)
		for(j=y1;j<y2;j++)
			draw_pixel(i,j,mode);

}

    

void graphics_Glyph(unsigned char left, unsigned char top,
			   unsigned char width, unsigned char height,
			   const unsigned char *glyph, unsigned char store_width)
{
	unsigned char x,b,glyph_idx, bitmask;
	int8_t y;
	const unsigned char *glyph_scan = glyph;

	for (x=0; x<width; x++)
	{
		for (y=0; y<height; y++)
		{
			glyph_idx = (x / 8) + y*store_width;
			b = glyph_scan[glyph_idx];
			bitmask = 1<<(7-(x % 8));
			if (b & bitmask)
				draw_pixel(left-width+x,top+y,1);
			else 
				draw_pixel(left-width+x,top+y,0);
		}
	}
}

void graphics_text(unsigned char left, unsigned char top, unsigned char font, char *str)
{
	unsigned char x = left;
	unsigned char glyph;
 	unsigned char width;
	unsigned char height;
	unsigned char store_width;
	const unsigned char *glyph_ptr;
	
  while(*str != 0x00)
  {

		glyph = (unsigned char)*str;

		/* check to make sure the symbol is a legal one */
		/* if not then just replace it with the default character */
		if((glyph < fonts[font].glyph_beg) || (glyph > fonts[font].glyph_end))
		{
			glyph = fonts[font].glyph_def;
		}
		

		/* make zero based index into the font data arrays */
		glyph -= fonts[font].glyph_beg;
		width = fonts[font].fixed_width;	/* check if it is a fixed width */
		if(width == 0)
		{
			width=fonts[font].width_table[glyph];	/* get the variable width instead */
		}

		height = fonts[font].glyph_height;
		store_width = fonts[font].store_width;

		glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);


		x+=width;
		graphics_Glyph(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */
		str++;								/* point to next character in string */
	}
	
}

void graphics_Glyph_vertical(unsigned char left, unsigned char top,
			   unsigned char width, unsigned char height,
			   const unsigned char *glyph, unsigned char store_width)
{
	unsigned char x,b,glyph_idx, bitmask;
	int8_t y;
	const unsigned char *glyph_scan = glyph;

	for (x=0; x<width; x++)
	{
		for (y=0; y<height; y++)
		{
			glyph_idx = (x / 8) + y*store_width;
			b = glyph_scan[glyph_idx];
			bitmask = 1<<(7-(x % 8));
			if (b & bitmask)
				draw_pixel(top+y,63-(left-width+x),1);
			else 
				draw_pixel(top+y,63-(left-width+x),0);
		}
	}
}

void graphics_text_vertical(unsigned char left, unsigned char top, unsigned char font, char *str)
{
	unsigned char x = left;
	unsigned char glyph;
 	unsigned char width;
	unsigned char height;
	unsigned char store_width;
	const unsigned char *glyph_ptr;
	
  while(*str != 0x00)
  {

		glyph = (unsigned char)*str;

		/* check to make sure the symbol is a legal one */
		/* if not then just replace it with the default character */
		if((glyph < fonts[font].glyph_beg) || (glyph > fonts[font].glyph_end))
		{
			glyph = fonts[font].glyph_def;
		}
		

		/* make zero based index into the font data arrays */
		glyph -= fonts[font].glyph_beg;
		width = fonts[font].fixed_width;	/* check if it is a fixed width */
		if(width == 0)
		{
			width=fonts[font].width_table[glyph];	/* get the variable width instead */
		}

		height = fonts[font].glyph_height;
		store_width = fonts[font].store_width;

		glyph_ptr = fonts[font].glyph_table + ((unsigned int)glyph * (unsigned int)store_width * (unsigned int)height);


		x+=width;
		graphics_Glyph_vertical(x,top,width,height,glyph_ptr,store_width);  /* plug symbol into buffer */
		str++;								/* point to next character in string */
	}
	
}

void draw_bitmap(uint8_t left, uint8_t top, const unsigned char *bitmap)
{
	uint16_t width = bitmap[1]<<8 | bitmap[0];
	uint16_t height = bitmap[3]<<8 | bitmap[2];
	
	uint8_t i,j,k;
	uint8_t byte;
	uint8_t x,y;
	for(k=0;k<=(height/8);k++)
	{
		for(i=0;i<=width;i++)
		{
			byte = bitmap[i+3+k*width];
			for(j=0;j<8;j++)
			{
				x = left+i-1;
				y = top+j+(k<<3);
				if(x-left>=0 && x-left<width && y-top>=0 && y-top<height) //clipping
				{
					if(byte&(0x01<<j))
						draw_pixel(x,y,1);
					else
						draw_pixel(x,y,0);
				}
			}
		}
	}
}

void draw_bitmap_fast(uint8_t left, uint8_t top, const unsigned char *bitmap)
{
	uint16_t width = bitmap[1]<<8 | bitmap[0];
	uint16_t height = bitmap[3]<<8 | bitmap[2];
	uint8_t i,j,k=3;
	for(i=0;i<=(height>>3);i++)
		for(j=0;j<width;j++)
		{
			k++;
			gddram[i+top][j+left] = bitmap[k];
		}
}


void draw_line( unsigned char x1, unsigned char y1, 
		unsigned char x2, unsigned char y2,
		unsigned char mode ) 
{
	unsigned char length, xTmp, yTmp, i, y, yAlt;
	int m;
	
	if(x1 == x2) {									// vertical line
		// x1|y1 must be the upper point
		if(y1 > y2) {
			xTmp = x1;
			yTmp = y1;
			x1 = x2;
			y1 = y2;
			x2 = xTmp;
			y2 = yTmp;
		}
		
		length = y2-y1;
		for(i=0; i<=length; i++) {
			draw_pixel(x1, y1+i, mode);
		}
	} else if(y1 == y2) {							// horizontal line
		// x1|y1 must be the left point
		if(x1 > x2) {
			xTmp = x1;
			yTmp = y1;
			x1 = x2;
			y1 = y2;
			x2 = xTmp;
			y2 = yTmp;
		}
		
		length = x2-x1;
		for(i=0; i<=length; i++) {
			draw_pixel(x1+i, y1, mode);
		}
	} else {
		// x1 must be smaller than x2
		if(x1 > x2) {
			xTmp = x1;
			yTmp = y1;
			x1 = x2;
			y1 = y2;
			x2 = xTmp;
			y2 = yTmp;
		}
		
		if((y2-y1) >= (x2-x1) || (y1-y2) >= (x2-x1)) {	// angle larger or equal 45°
			length = x2-x1;								// not really the length :)
			m = ((y2-y1)*200)/length;
			yAlt = y1;
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+y1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				draw_line(x1+i, yAlt, x1+i, y, mode ); /* wuff wuff recurs. */
				if(length<=(y2-y1) && y1 < y2)
					yAlt = y+1;
				else if(length<=(y1-y2) && y1 > y2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		} else {										// angle smaller 45°
			// y1 must be smaller than y2
			if(y1 > y2) {
				xTmp = x1;
				yTmp = y1;
				x1 = x2;
				y1 = y2;
				x2 = xTmp;
				y2 = yTmp;
			}
			length = y2-y1;
			m = ((x2-x1)*200)/length;
			yAlt = x1;
			for(i=0; i<=length; i++) {
				y = ((m*i)/200)+x1;
				
				if((m*i)%200 >= 100)
					y++;
				else if((m*i)%200 <= -100)
					y--;
				
				draw_line(yAlt, y1+i, y, y1+i, mode); 
				if(length<=(x2-x1) && x1 < x2)
					yAlt = y+1;
				else if(length<=(x1-x2) && x1 > x2)
					yAlt = y-1;
				else
					yAlt = y;
			}
		}
	}
}

//void rotate_point(float *x, float *y, float angle, float about_x, float about_y)
//{
//	float xtmp,ytmp;
//	xtmp = *x-about_x;
//	ytmp = *y-about_y;
//	
//	*x = (float)(xtmp*cos(angle*0.01745) - ytmp*sin(angle*0.01745) + about_x);
//	*y = (float)(xtmp*sin(angle*0.01745) +ytmp*cos(angle*0.01745) + about_y);
//}

uint8_t stevec = 0;
uint8_t init_stevec = 0;
char buf[50];
char buf2[20];
uint16_t start_index;
uint16_t end_index;
float display_factor = 1000;
uint8_t display_state = 0;
float spidura_temp = 0;

extern struct joystick_t joystick;
void draw_screen(void)
{
	if(init_stevec<60)
	{
		init_stevec++;
		graphics_text(0,0,FONT_NINE_DOT,"WARTEN SIE BITTE");
		graphics_text(0,12,FONT_SEVEN_DOT,"Die Mikrowelle");
		graphics_text(0,20,FONT_SEVEN_DOT,"wird aufgewarmt!");
		draw_pixel(75,20,add);
		draw_pixel(77,20,add);
		draw_rectangle(7,120,45,55,add);
		draw_rectangle(9,118,47,53,erase);
		line_h(10,(init_stevec+10)*1.685,50,4,add);
	}
	else if(GetShutdownTimer() != 0)
	{
		graphics_text(35,0,FONT_NINE_DOT,"HOLD TO");
		graphics_text(28,12,FONT_NINE_DOT,"POWER OFF");
		graphics_text(24,27,FONT_NINE_DOT,"RELEASE TO");
		graphics_text(24,39,FONT_NINE_DOT,"ENTER MENU");
		draw_pixel(75,20,add);
		draw_pixel(77,20,add);
		draw_rectangle(7,120,53,63,add);
		draw_rectangle(9,118,55,61,erase);
		line_h(10,(GetShutdownTimer()*4+10),58,4,add);
	}
	else
	{
		if(joystick.left)
			display_state = 0;
		if(joystick.right)
			display_state = 1;
		
		if(joystick.up)
		{
			display_factor -= 100;
			if(display_factor<100)
				display_factor = 100;
		}
		
		if(joystick.down)
		{
			display_factor += 100;
			if(display_factor>5000)
				display_factor = 5000;
		}
		
		end_index = start_index+128; //calc end index (128px on the display)
		uint16_t i;
		float spidura = get_speediro();
		uint16_t peak_index = get_peakindex();
		

		/* Display data graph on LCD */
		switch(display_state) //low speeds
		{
			case 0 :
				for (i = 0; i < 128; i++) 
				{
					float buff = Bars[i];
					buff = buff/(10*display_factor);
					if(buff>55)
						buff = 55;
					line_v(55,55-buff,i,1,add);
				}
			
			
				for(i=3;i<128;i+=20)
				{
					sprintf(buf2,"%.1f",i*factor);
					graphics_text(i-3,58,FONT_SIX_DOT,buf2);
				} 
				graphics_text(0,0,FONT_SEVEN_DOT,"RNG:");
				graphics_text(0,10,FONT_SEVEN_DOT,"0-7");
				graphics_text(0,20,FONT_SEVEN_DOT,"m/s");
				
				break;
			case 1 :
				for (i = 85; i < 597; i+=4) 
				{
					float buff = Bars[i];
					buff = buff/1500;
					if(buff>64)
						buff = 64;
					line_v(55,55-buff,(i-85)/4,1,add);
				}
			
			
				for(i=99;i<550;i+=82)
				{
					float buff2 = (i)*factor;
					sprintf(buf2,"%.0f",buff2);
					graphics_text((i-85-12)/4,58,FONT_SIX_DOT,buf2);
					line_v(55,57,(i-85)/4,1,add);
				} 
				
				graphics_text(0,0,FONT_SEVEN_DOT,"RNG:");
				graphics_text(0,10,FONT_SEVEN_DOT,"6-35");
				graphics_text(0,20,FONT_SEVEN_DOT,"m/s");
				
				break;
		}

		
		if(stevec%3==0)
		{
			spidura_temp = spidura;			
		}
		
		sprintf(buf, "%.2f",spidura_temp);
		if(spidura_temp<10)
		{
			
			graphics_text(46,0,FONT_EIGHTEEN_DOT,buf);
			graphics_text(105,8,FONT_NINE_DOT,"m/s");
		}
		else
		{
			graphics_text(30,0,FONT_EIGHTEEN_DOT,buf);
			graphics_text(105,8,FONT_NINE_DOT,"m/s");
		}
		
		
//		sprintf(buf2,"%.0f%%",100-(display_factor-100)/49);
//		graphics_text(80,35,FONT_SIX_DOT,"ZOOM:");
//		graphics_text(105,35,FONT_SIX_DOT,buf2);
		if(spidura_temp == 0)
			graphics_text(60,23,FONT_SIX_DOT,"LOW SIGNAL");
		stevec++;
		
		
		//draw_rectangle(peak_index-3 < 0 ? 0 :  peak_index-3,(peak_index+1),62-Bars[peak_index]/15000,66-Bars[peak_index]/15000,add);
	}
}
