/*******************************************************************************/
/**	Description: 															  **/
/**	  fonts.h - LCD fonts	 header file									  **/
/*******************************************************************************/
/**  Compiler	: 	Keil CARM V2.4											  **/
/**	 Target 	:	Philips LPC2138											  **/
/**																			  **/
/**	 Code portions Copyright (C) 2004 Martin THOMAS							  **/
/**  Author		: 	Marko Pavlin, 											  **/
/**					marko.pavlin@guest.arnes.si								  **/
/*******************************************************************************/
/**	Revision history: 														  **/
/**	-----------------														  **/
/**	  v0.0 20.12.2005 - file created										  **/
/**																			  **/
/**																			  **/
/*******************************************************************************/

							    

#ifndef _FONTS_H												   
#define _FONTS_H

/* select desired fonts. (Simply comment out those not needed) */
#define EN_FIVE_DOT
#define EN_SIX_DOT
#define EN_SEVEN_DOT
//#define EN_NINE_DOT
//#define EN_TEN_DOT
#define EN_FIFTEEN_DOT
//#define EN_EIGHTEEN_DOT
//#define EN_FONT_MT1

/* define number labels for the font selections */
typedef enum
{
#ifdef EN_FIVE_DOT
	FONT_FIVE_DOT,
#endif

#ifdef EN_SIX_DOT
	FONT_SIX_DOT,
#endif

#ifdef EN_SEVEN_DOT
	FONT_SEVEN_DOT,
#endif

#ifdef EN_NINE_DOT
	FONT_NINE_DOT,
#endif

#ifdef EN_TEN_DOT
	FONT_TEN_DOT,
#endif

#ifdef EN_FIFTEEN_DOT
	FONT_FIFTEEN_DOT,
#endif

#ifdef EN_EIGHTEEN_DOT
	FONT_EIGHTEEN_DOT,
#endif

#ifdef EN_FONT_MT1
	FONT_MT1,
#endif

	FONT_COUNT
} FONT_BASE;

struct FONT_DEF 
{
   unsigned char store_width;            /* glyph storage width in bytes */
   unsigned char glyph_height;  		 /* glyph height for storage */
   const unsigned char *glyph_table;      /* font table start address in memory */
   unsigned char fixed_width;            /* fixed width of glyphs. If zero */
                                         /* then use the width table. */
   const unsigned char *width_table; 	 /* variable width table start adress */
   unsigned char glyph_beg;			 	 /* start ascii offset in table */
   unsigned char glyph_end;				 /* end ascii offset in table */
   unsigned char glyph_def;				 /*  for undefined glyph  */
};

/* font definition tables for the fonts */
extern const struct FONT_DEF fonts[FONT_COUNT];

/* glyph bitmap and width tables for the fonts */ 
#ifdef EN_FIVE_DOT
  extern const unsigned char five_dot_glyph_table[];
  extern const unsigned char five_dot_width_table[];
#endif

#ifdef EN_SIX_DOT
  extern const unsigned char six_dot_glyph_table[];
  extern const unsigned char six_dot_width_table[];
#endif

#ifdef EN_SEVEN_DOT
#define DEG_CHAR ('~'+1)
  extern const unsigned char seven_dot_glyph_table[];
  extern const unsigned char seven_dot_width_table[];
#endif

#ifdef EN_NINE_DOT
  extern const unsigned char  nine_dot_glyph_table[];
#endif

#ifdef EN_TEN_DOT
  extern const unsigned char  ten_dot_glyph_table[];
#endif

#ifdef EN_FIFTEEN_DOT
  extern const unsigned char  fifteen_dot_glyph_table[];
  extern const unsigned char  fifteen_dot_width_table[];
#endif

#ifdef EN_EIGHTEEN_DOT
  extern const unsigned char  eighteen_dot_glyph_table[];
  extern const unsigned char  eighteen_dot_width_table[];
#endif

#ifdef EN_FONT_MT1
  extern const unsigned char  font_mt1_glyph_table[];
  extern const unsigned char  font_mt1_width_table[];
#endif

#endif
