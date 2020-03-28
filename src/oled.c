#include <string.h>

#include "../inc/sh1106_registers.h"
#include "../inc/oled.h"

#if (defined(SH1106) || defined(SSD1306)) && defined(INTERFACE_I2C)

	#define OLED_BUFFER_SIZE ((OLED_HEIGHT>>3)*(OLED_WIDTH+1))
	#define OLED_CONTROL_BYTE_SIZE 1
	#define OLED_I2C_DATA    0x40
	#define OLED_I2C_COMMAND 0x00
	#define COMMAND_BUFFER_SIZE 4

#else

	#define OLED_BUFFER_SIZE ((OLED_HEIGHT>>3)*(OLED_WIDTH))
	#define COMMAND_BUFFER_SIZE 3

#endif

static uint8_t OLED_displayBuffer[OLED_BUFFER_SIZE];
static uint8_t OLED_commandsBuffer[COMMAND_BUFFER_SIZE];

static OLED_CommunicationInterface_t localInterface;


void OLED_init(OLED_CommunicationInterface_t * interfaceConfig)
{

	memcpy(&localInterface, interfaceConfig, sizeof(OLED_CommunicationInterface_t));

#if defined(INTERFACE_SPI)
	localInterface.OLED_hwReset();
	localInterface.OLED_delayMS(100);
#endif

	// turn off OLED panel
	OLED_writeCommand(SH1106_COMMAND_SET_DISPLAY_ONOFF | 0x00);

	// set all control register to POR values
	OLED_writeCommand(SH1106_COMMAND_SET_LOWER_COLUMN_ADDRESS | 0x02 );
	OLED_writeCommand(SH1106_COMMAND_SET_HIGHER_COLUMN_ADDRESS | 0x00 );
	OLED_writeCommand(SH1106_COMMAND_SET_PUMP_VOLTAGE | 0x02);
	OLED_writeCommand(SH1106_COMMAND_SET_DISPLAY_START_LINE | 0x00);
	OLED_writeCommand(SH1106_COMMAND_SET_SEGMENT_REMAP | 0x01);
	OLED_writeCommand(SH1106_COMMAND_SET_ENTIRE_DISPLAY_ONOFF | 0x00);
	OLED_writeCommand(SH1106_COMMAND_SET_NORMALREVERSE_DISPLAY | 0x00);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_MULTIPLEX_RATIO, 0x3F);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_DCDC_CONTROL, 0x8B);
	OLED_writeCommand(SH1106_COMMAND_SET_PAGE_ADRESS | 0x00);
	OLED_writeCommand(SH1106_COMMAND_SET_COMMON_OUTPUT_SCAN_DIR | 0x08);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_DISPLAY_OFFSET, 0x00);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_CLOCK_DIVIDE_RATIO_AND_OSCILLATOR, 0x50);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_DIS_PRE_CHARGE_PERIOD, 0x22);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_PADS_HARDWARE_CONFIG, 0x12);
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_VCOM_DESELECT_LEVEL_MODE, 0x35);
	OLED_writeCommand(SH1106_COMMAND_SET_DISPLAY_ONOFF | 0x01);
	//Set contrast to maximum
	OLED_writeDoubleCommand(SH1106_COMMAND_SET_CONTRAST_CONTROL_REG, 0xFF);
	//clear screen
	OLED_clrScreen(0x00);
}

void OLED_writeCommand(uint8_t command)
{
#if defined(INTERFACE_I2C)
	OLED_commandsBuffer[0] = OLED_I2C_COMMAND;
	OLED_commandsBuffer[1] = command;
	localInterface.OLED_InterfaceWriteNBytes(OLED_I2C_ADDR,OLED_commandsBuffer,2);
#elif defined(INTERFACE_SPI)
	localInterface.OLED_chipSelectLow();
	localInterface.OLED_comandMode();
	OLED_commandsBuffer[0] = command;
	localInterface.OLED_InterfaceWriteNBytes(&OLED_commandsBuffer, 1);
	localInterface.OLED_chipSelectHigh();
#endif
}

void OLED_writeDoubleCommand(uint8_t commandA, uint8_t commandB)
{
#if defined(INTERFACE_I2C)
	OLED_commandsBuffer[0] = OLED_I2C_COMMAND;
	OLED_commandsBuffer[1] = commandA;
	OLED_commandsBuffer[2] = commandB;
	localInterface.OLED_InterfaceWriteNBytes(OLED_I2C_ADDR,OLED_commandsBuffer,3);
#elif defined(INTERFACE_SPI)
	localInterface->OLED_chipSelectLow();
	localInterface->OLED_comandMode();
	OLED_commandsBuffer[0] = commandA;
	OLED_commandsBuffer[1] = commandB;
	localInterface->OLED_InterfaceWriteNBytes(&OLED_commandsBuffer, 2);
	localInterface->OLED_chipSelectHigh();
#endif
}

void OLED_writeTripleCommand(uint8_t commandA, uint8_t commandB, uint8_t commandC)
{
#if defined(INTERFACE_I2C)
	OLED_commandsBuffer[0] = OLED_I2C_COMMAND;
	OLED_commandsBuffer[1] = commandA;
	OLED_commandsBuffer[2] = commandB;
	OLED_commandsBuffer[3] = commandC;
	localInterface.OLED_InterfaceWriteNBytes(OLED_I2C_ADDR,OLED_commandsBuffer,4);
#elif defined(INTERFACE_SPI)
	localInterface.OLED_chipSelectLow();
	localInterface.OLED_comandMode();
	OLED_commandsBuffer[0] = commandA;
	OLED_commandsBuffer[1] = commandB;
	OLED_commandsBuffer[2] = commandC;
	localInterface.OLED_InterfaceWriteNBytes(&OLED_commandsBuffer, 3);
	localInterface.OLED_chipSelectHigh();
#endif
}

void OLED_clrScreen(uint8_t pattern)
{
	memset(&OLED_displayBuffer[0], pattern, OLED_BUFFER_SIZE);
	OLED_updateScreen();
}

void OLED_updateScreen(void)
{
	uint8_t * dataBufPtr = 0;
	dataBufPtr = OLED_displayBuffer;

#if defined(INTERFACE_I2C)

	for(uint8_t i = 0; i < (OLED_HEIGHT>>3) ; i++)
	{
		*dataBufPtr = OLED_I2C_DATA;
#if defined(SH1106)
		OLED_writeTripleCommand(SH1106_COMMAND_SET_PAGE_ADRESS | i, SH1106_COMMAND_SET_LOWER_COLUMN_ADDRESS | 0x02, SH1106_COMMAND_SET_HIGHER_COLUMN_ADDRESS );
#endif
		localInterface.OLED_InterfaceWriteNBytes(OLED_I2C_ADDR, dataBufPtr, OLED_WIDTH + OLED_CONTROL_BYTE_SIZE);
		dataBufPtr += OLED_WIDTH + OLED_CONTROL_BYTE_SIZE;
	}

#elif defined(INTERFACE_SPI)

	localInterface.OLED_chipSelectLow();
	localInterface.OLED_dataMode();

	for(uint8_t i = 0; i < (OLED_HEIGHT>>3) ; i++)
	{
		*dataBufPtr = OLED_I2C_DATA;
#if defined(SH1106)
		OLED_writeTripleCommand(SH1106_COMMAND_SET_PAGE_ADRESS | i, SH1106_COMMAND_SET_LOWER_COLUMN_ADDRESS | 0x02, SH1106_COMMAND_SET_HIGHER_COLUMN_ADDRESS );
#endif
		localInterface.OLED_InterfaceWriteNBytes(dataBufPtr, OLED_WIDTH);
		dataBufPtr += OLED_WIDTH;
	}

	localInterface.OLED_chipSelectHigh();

#endif
}

uint8_t * OLED_getDisplayBufferPointer(uint8_t x, uint8_t line)
{
#if defined(INTERFACE_I2C)
	return OLED_displayBuffer + ((line * OLED_WIDTH)) + ((line+1)*OLED_CONTROL_BYTE_SIZE) + x;
#elif defined(INTERFACE_SPI)
	return OLED_displayBuffer + ((line * OLED_WIDTH)) + x;
#endif
}

