/*
 * SSD1306_gfx.c
 *
 * Created: 13.11.2018 20:06:24
 *  Author: rafal
 */

#include <stdint.h>

#include "../inc/oled.h"


void OLED_GFX_putPixel(uint8_t x, uint8_t y)
{
	uint8_t * displayLinePtr = OLED_getDisplayBufferPointer(x, y>>3);
	if(( x > (OLED_WIDTH-1)) || (y > (OLED_HEIGHT-1))) return;
	*displayLinePtr |= (1<<y%8);
}

void OLED_GFX_clrPixel (uint8_t x, uint8_t y)
{	
	uint8_t * displayLinePtr = OLED_getDisplayBufferPointer(x, y>>3);
	if(( x > (OLED_WIDTH-1)) || (y > (OLED_HEIGHT-1))) return;
	*displayLinePtr &= ~(1<<(y%8));
}

void OLED_GFX_drawLine(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, OLED_GFX_color_t color )
{
	int x0,x1,y0,y1;
	int dx, dy;

	x0 = (int)x_0;
	x1 = (int)x_1;
	y0 = (int)y_0;
	y1 = (int)y_1;
	
	void (*function_ptr)(uint8_t x, uint8_t y);
	
	if(color) function_ptr = OLED_GFX_putPixel;
	else function_ptr = OLED_GFX_clrPixel;
	
	if( x1 <= x0 ) dx = x0 - x1;
	else dx = x1 - x0;
	if( y1 <= y0 ) dy = x0 - x1;
	else dy = y1 - y0;

	int sx = x0<x1 ? 1 : -1;

	int sy = y0<y1 ? 1 : -1;

	int err = (dx>dy ? dx : -dy)/2, e2;

	while(1)
	{
		function_ptr(x0,y0);
		if (x0==x1 && y0==y1) break;
		e2 = err;
		if (e2 >-dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}



void OLED_GFX_drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, OLED_GFX_color_t color)
{
	OLED_GFX_drawLine(x,y,x+width,y,color);
	OLED_GFX_drawLine(x+width,y,x+width,y+height,color);
	OLED_GFX_drawLine(x,y+height,x+width,y+height,color);
	OLED_GFX_drawLine(x,y,x,y+height,color);
}

void OLED_GFX_drawFilledRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height,OLED_GFX_color_t color)
{
	union pattern
	{
		uint64_t bits;
		uint8_t  tab[8];
	};

	union pattern pat;

	uint8_t * displayBufPtr;

	pat.bits = 0xFFFFFFFFFFFFFFFFULL;
	pat.bits =  (uint64_t)((uint64_t)pat.bits << (uint64_t)y);
	pat.bits &= (uint64_t)((uint64_t)pat.bits >> (uint64_t)(64-height-y));

	for(uint8_t j = 0; j < 8; j++ )
	{
		displayBufPtr = OLED_getDisplayBufferPointer(0,j);

		for(uint8_t i = x; i < (x + width); i++)
		{
			if(color)
			{
				displayBufPtr[i] |= pat.tab[j];
			}
			else
			{
				displayBufPtr[i] &= ~pat.tab[j];
			}
		}
	}


}

void OLED_GFX_drawFilledSquare(uint8_t x, uint8_t y, uint8_t a, OLED_GFX_color_t color)
{
	OLED_GFX_drawFilledRect(x,y,a,a,color);
}


void OLED_GFX_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, OLED_GFX_color_t color)
{

	int x = (int)(radius);
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (int)(radius << 1);
	
	void (*function_ptr)(uint8_t x, uint8_t y);
	
	if(color) function_ptr = OLED_GFX_putPixel;
	else function_ptr = OLED_GFX_clrPixel;

	while (x >= y)
	{
		function_ptr((uint8_t)(x0 + x), (uint8_t)(y0 + y));
		function_ptr((uint8_t)(x0 + y), (uint8_t)(y0 + x));
		function_ptr((uint8_t)(x0 - y), (uint8_t)(y0 + x));
		function_ptr((uint8_t)(x0 - x), (uint8_t)(y0 + y));
		function_ptr((uint8_t)(x0 - x), (uint8_t)(y0 - y));
		function_ptr((uint8_t)(x0 - y), (uint8_t)(y0 - x));
		function_ptr((uint8_t)(x0 + y), (uint8_t)(y0 - x));
		function_ptr((uint8_t)(x0 + x), (uint8_t)(y0 - y));

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (radius << 1);
		}
	}
}

void OLED_GFX_drawFilledCircle(uint8_t x0, uint8_t y0, uint8_t radius, OLED_GFX_color_t color)
{
	int x = (int)(radius);
	int r2 = x << 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - r2;

	while (x >= y)
	{
		OLED_GFX_drawLine( (uint8_t)(x0 - x), (uint8_t)(y0 + y), (uint8_t)(x0 + x), (uint8_t)(y0 + y), color );
		OLED_GFX_drawLine( (uint8_t)(x0 - x), (uint8_t)(y0 - y), (uint8_t)(x0 + x), (uint8_t)(y0 - y), color );
		OLED_GFX_drawLine( (uint8_t)(x0 - y), (uint8_t)(y0 - x), (uint8_t)(x0 + y), (uint8_t)(y0 - x), color );
		OLED_GFX_drawLine( (uint8_t)(x0 - y), (uint8_t)(y0 + x), (uint8_t)(x0 + y), (uint8_t)(y0 + x), color );
		
		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - r2;
		}
	}
}

void OLED_GFX_drawArc(uint8_t x0, uint8_t y0, uint8_t r, uint8_t one_eight, OLED_GFX_color_t color )
{
	int x = (int)(r);
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (int)(r << 1);
	
	void (*function_ptr)(uint8_t x, uint8_t y);
	
	if(color) function_ptr = OLED_GFX_putPixel;
	else function_ptr = OLED_GFX_clrPixel;

	while (x >= y)
	{
		switch(one_eight)
		{
			case 0: function_ptr((uint8_t)(x0 + x), (uint8_t)(y0 - y)); break;
			case 1: function_ptr((uint8_t)(x0 + y), (uint8_t)(y0 - x)); break;
			case 2: function_ptr((uint8_t)(x0 - y), (uint8_t)(y0 - x)); break;
			case 3: function_ptr((uint8_t)(x0 - x), (uint8_t)(y0 - y)); break;
			case 4: function_ptr((uint8_t)(x0 - x), (uint8_t)(y0 + y)); break;
			case 5: function_ptr((uint8_t)(x0 - y), (uint8_t)(y0 + x)); break;
			case 6: function_ptr((uint8_t)(x0 + y), (uint8_t)(y0 + x)); break;
			case 7: function_ptr((uint8_t)(x0 + x), (uint8_t)(y0 + y)); break;
		}

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}

		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

void OLED_GFX_drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t corner_radius, OLED_GFX_color_t color)
{
	if( ((corner_radius) > (width>>1)) || ((corner_radius) > (height>>1)) ) return;

	uint8_t r = corner_radius; 

	OLED_GFX_drawArc( x+width-r, y + r, r , 0, color);
	OLED_GFX_drawArc( x+width-r, y + r, r , 1, color);
	OLED_GFX_drawArc( x + r, y + r, r , 2, color);
	OLED_GFX_drawArc( x + r, y + r, r , 3, color);
	OLED_GFX_drawArc( x+width-r, y+height-r, r , 6, color);
	OLED_GFX_drawArc( x+width-r, y+height-r, r , 7, color);
	OLED_GFX_drawArc( x+r, y+height-r, r , 4, color);
	OLED_GFX_drawArc( x+r, y+height-r, r , 5, color);

	OLED_GFX_drawLine( x+r, y, x+width-r , y , color);
	OLED_GFX_drawLine( x+width, y + r , x+width , y+height - r, color );
	OLED_GFX_drawLine( x+r, y+height , x+width-r , y+height, color );
	OLED_GFX_drawLine( x, y+r, x, y+height-r, color);
}


void OLED_GFX_loadBitmapFromFlash(const uint8_t * otr)
{
	uint8_t * displayBufPtr;

	for(uint8_t j = 0; j < (OLED_HEIGHT>>3); j++)
	{
		displayBufPtr = OLED_getDisplayBufferPointer(0,j);

		for(uint8_t i = 0; i < (OLED_WIDTH); i++)
		{
			*displayBufPtr++ = *otr++;
		}		
	}
}

void OLED_GFX_loadSmallBitmap(const uint8_t *picture, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y)
{
	uint8_t current_byte;
	
	for(uint8_t y = 0; y < height; y++)
	{
		for(uint8_t x = 0; x < width; x++)
		{	
			current_byte = *(picture + y*(width/8) + x/8);
			
			if(0x80 & (current_byte<<(x%8)))
			{
				OLED_GFX_putPixel(pos_x + x, pos_y + y);
			}
			else
			{
				OLED_GFX_clrPixel(pos_x + x, pos_y + y);
			}
		}
	}
}
		


