/*
 * @file    ssd1306_registers.h
 * @brief   This file contains human readable names for internal
 *          SSD1306/SH1106 registers
 * @author  Nefarious19
 */
#ifndef SH1106_REGISTERS_H_
#define SH1106_REGISTERS_H_

#define SH1106_COMMAND_SET_LOWER_COLUMN_ADDRESS  	          0x00 // set only [3:0] bits (range from 0x0 to 0xF)
#define SH1106_COMMAND_SET_HIGHER_COLUMN_ADDRESS 	          0x10 // set only [3:0] bits (range from 0x0 to 0xF)
#define SH1106_COMMAND_SET_PUMP_VOLTAGE				          0x30 // set only [1:0] bits (range from 0x0 to 0x3)
#define SH1106_COMMAND_SET_DISPLAY_START_LINE		          0x40 // set only [5:0] bits (range from 0x0 to 0x3F)
#define SH1106_COMMAND_SET_CONTRAST_CONTROL_REG		          0x81 // double byte command, next byte is contrast value in range 0x0 - 0xFF
#define SH1106_COMMAND_SET_SEGMENT_REMAP			          0xA0 // set only [0] bit (range from 0x0 to 0x1)
#define SH1106_COMMAND_SET_ENTIRE_DISPLAY_ONOFF               0xA4 // set only [0] bit (range from 0x0 to 0x1)
#define SH1106_COMMAND_SET_NORMALREVERSE_DISPLAY	          0xA6 // set only [0] bit (range from 0x0 to 0x1)
#define SH1106_COMMAND_SET_MULTIPLEX_RATIO			          0xA8 // double byte command, next byte is multiplex ratio in range 0x0 - 0x3F
#define SH1106_COMMAND_SET_DCDC_CONTROL				          0xAD // double byte command, next byte is multiplex ratio in range 0x8A - 0x8B
#define SH1106_COMMAND_SET_DISPLAY_ONOFF			          0xAE // set only [0] bit (range from 0x0 to 0x1)
#define SH1106_COMMAND_SET_PAGE_ADRESS				          0xB0 // // set only [2:0] bits (range from 0x0 to 0x7)
#define SH1106_COMMAND_SET_COMMON_OUTPUT_SCAN_DIR	          0xC0 // set only [3] bit ( 0x00 to 0x08 ) (flips verticaly display)
#define SH1106_COMMAND_SET_DISPLAY_OFFSET			          0xD3 // double byte command, next byte is first line offset in range 0x0 - 0x3F
#define SH1106_COMMAND_SET_CLOCK_DIVIDE_RATIO_AND_OSCILLATOR  0xD5 // double byte command, next byte [7:4] bits are setting clock divide ratio, [3:0] are setting oscilator frequency
#define SH1106_COMMAND_SET_DIS_PRE_CHARGE_PERIOD			  0xD9 // double byte command, next byte [7:4] bits are setting pre-charge period, [3:0] are setting dis-charge period
#define SH1106_COMMAND_SET_PADS_HARDWARE_CONFIG				  0xDA // set only [4] bit ( 0x10 to 0x12 )
#define SH1106_COMMAND_SET_VCOM_DESELECT_LEVEL_MODE           0xDB // double byte command, next byte [7:0] sets VCOM voltage level
#define SH1106_COMMAND_NOP									  0xE3 // do nothing command


#endif /* SH1106_REGISTERS_H_ */
