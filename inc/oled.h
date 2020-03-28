#ifndef OLED_H_
#define OLED_H_

#include <stdint.h>

/* choose your OLED driver IC symbol */
#define SH1106
//#define SSD1306

/* choose type of communication interface */
#define INTERFACE_SPI
//#define INTERFACE_I2C

/* define size of your OLED */
#define OLED_WIDTH    0x80    //128 px
#define OLED_HEIGHT	  0x40    //64  px


/* Defines listed below are only used to calculate proper line height and position
 *
 */
#define OLED_FONT_WIDTH 	5
#define OLED_FONT_HEIGHT	7

#define OLED_CHAR_WIDTH   (OLED_FONT_WIDTH + 2)
#define OLED_CHAR_HEIGHT  (OLED_FONT_HEIGHT + 3)

typedef enum
{
	OLED_COLOR_BLACK,
	OLED_COLOR_WHITE
}OLED_GFX_color_t;

/* enum OLED_PRT_LineNo can be used as y parameter of printing
 * API.
 */

enum OLED_PRT_LineNo
{
	OLED_LINE1 = 0,
	OLED_LINE2 = OLED_CHAR_HEIGHT*1,
	OLED_LINE3 = OLED_CHAR_HEIGHT*2,
	OLED_LINE4 = OLED_CHAR_HEIGHT*3,
	OLED_LINE5 = OLED_CHAR_HEIGHT*4,
	OLED_LINE6 = OLED_CHAR_HEIGHT*5,
};

#if defined(INTERFACE_I2C)

	#define	OLED_I2C_ADDR 0x78

	typedef struct
	{
		void ( * OLED_InterfaceWriteNBytes)(uint8_t i2cAddress, uint8_t * dataPtr, uint16_t dataLen);
	}OLED_CommunicationInterface_t;

#elif defined(INTERFACE_SPI)

	typedef struct
	{
		void ( * OLED_hwReset)(void);
		void ( * OLED_InterfaceWriteNBytes)(uint8_t * dataPtr, uint16_t dataLen);
		void ( * OLED_chipSelectLow)(void);
		void ( * OLED_chipSelectHigh)(void);
		void ( * OLED_comandMode)(void);
		void ( * OLED_dataMode)(void);
		void ( * OLED_delayMS)(uint32_t delayInMilliseconds);
	}OLED_CommunicationInterface_t;

#endif


/*
 * BASIC API for controling the display.
 */
void OLED_init(OLED_CommunicationInterface_t * interfaceConfig);
uint8_t * OLED_getDisplayBufferPointer(uint8_t x, uint8_t line);
void OLED_writeCommand(uint8_t command);
void OLED_writeDoubleCommand(uint8_t commandA, uint8_t commandB);
void OLED_writeTripleCommand(uint8_t commandA, uint8_t commandB, uint8_t commandC);
void OLED_clrScreen(uint8_t pattern);
void OLED_updateScreen(void);

/*
 * Some basic graphical API for drawing on the display.
 */

void OLED_GFX_putPixel(uint8_t x, uint8_t y);
void OLED_GFX_drawLine(uint8_t x_0, uint8_t y_0, uint8_t x_1, uint8_t y_1, OLED_GFX_color_t color );
void OLED_GFX_drawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, OLED_GFX_color_t color);
void OLED_GFX_drawFilledRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, OLED_GFX_color_t color);
void OLED_GFX_drawFilledSquare(uint8_t x, uint8_t y, uint8_t a, OLED_GFX_color_t color);
void OLED_GFX_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, OLED_GFX_color_t color);
void OLED_GFX_drawFilledCircle(uint8_t x0, uint8_t y0, uint8_t radius, OLED_GFX_color_t color);
void OLED_GFX_drawArc(uint8_t x0, uint8_t y0, uint8_t r, uint8_t one_eight, OLED_GFX_color_t color );
void OLED_GFX_drawRoundedRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t corner_radius, OLED_GFX_color_t color);
void OLED_GFX_loadBitmapFromFlash(const uint8_t * otr);
void OLED_GFX_loadSmallBitmap(const uint8_t *picture, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y);



/*	Declaration of API that can be used for printing different
 *  types of data
 */
void OLED_PRT_putChr(char   data, uint8_t x, uint8_t y, uint8_t size );
void OLED_PRT_putStr(char * data, uint8_t x, uint8_t y, uint8_t size );
void OLED_PRT_putInt(int32_t num , uint8_t x, uint8_t y, uint8_t size );
void OLED_PRT_putHex(int32_t num , uint8_t x, uint8_t y, uint8_t size );
void OLED_PRT_putFloat( float num , uint8_t precision, uint8_t x, uint8_t y, uint8_t size );

#endif /* OLED_H_ */
