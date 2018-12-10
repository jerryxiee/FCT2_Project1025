/*******************(C)深圳市伊爱高新技术开发有限公司版权所有****************************
**  文件名称:   LCD_13264.c
**  创建作者:   欧恩惠
**  当前版本:   1.0.0.0
**  创建日期:   20130621
**  功能说明:   LCD13264驱动程序
**  修改记录:        
**  备    注:   
**********************************(C)copyright 2013**************************************/
#include "Lcd_13264.h"

//ASCII字库
#define ASCII_COUNT		128
u8 AsciiTab[ASCII_COUNT][16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0xFF,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x00,0xC0,0xE0,0xF0,0xF0,0xE0,0xC0,0x00,0x00,0x01,0x03,0x07,0x07,0x03,0x01,0x00,
0xFF,0x3F,0x1F,0x0F,0x0F,0x1F,0x3F,0xFF,0xFF,0xFE,0xFC,0xF8,0xF8,0xFC,0xFE,0xFF,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x84,0x64,0x1C,0x04,0x7C,0x00,0x1C,0x22,0x23,0x22,0x1C,0x00,0x00,
0x00,0x00,0x38,0x44,0xC4,0x44,0x38,0x00,0x00,0x02,0x02,0x02,0x3F,0x02,0x02,0x02,
0x00,0x00,0x00,0xFC,0x24,0x24,0x12,0xFE,0x00,0x30,0x38,0x1F,0x00,0x0C,0x0E,0x07,
0x00,0x88,0xF0,0x08,0x0E,0x08,0xF0,0x88,0x00,0x08,0x07,0x08,0x38,0x08,0x07,0x08,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0xFF,0x01,0x01,0x01,
0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF8,0xFC,0x00,0x01,0x03,0x07,0x0F,0x1F,0x3F,0x7F,
0x00,0x00,0x08,0x04,0xFE,0x04,0x08,0x00,0x00,0x00,0x10,0x20,0x7F,0x20,0x10,0x00,
0x00,0x00,0xFC,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x67,0x00,0x00,0x67,0x00,0x00,
0x00,0x70,0x88,0x04,0x04,0xFC,0x04,0xFC,0x00,0x00,0x00,0x01,0x01,0x3F,0x00,0x3F,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0xFF,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0xFF,0x00,0x00,0x00,
0x00,0x00,0x08,0x04,0xFE,0x04,0x08,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x40,0x80,0x00,0x00,0x01,0x01,0x01,0x01,0x05,0x03,0x01,
0x00,0x00,0x80,0x40,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x05,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x30,0x00,0x00,0x00,
0x00,0x10,0x0C,0x06,0x10,0x0C,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x40,0xC0,0x78,0x40,0xC0,0x78,0x40,0x00,0x04,0x3F,0x04,0x04,0x3F,0x04,0x04,0x00,
0x00,0x70,0x88,0xFC,0x08,0x30,0x00,0x00,0x00,0x18,0x20,0xFF,0x21,0x1E,0x00,0x00,
0xF0,0x08,0xF0,0x00,0xE0,0x18,0x00,0x00,0x00,0x21,0x1C,0x03,0x1E,0x21,0x1E,0x00,
0x00,0xF0,0x08,0x88,0x70,0x00,0x00,0x00,0x1E,0x21,0x23,0x24,0x19,0x27,0x21,0x10,
0x10,0x16,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0xE0,0x18,0x04,0x02,0x00,0x00,0x00,0x00,0x07,0x18,0x20,0x40,0x00,
0x00,0x02,0x04,0x18,0xE0,0x00,0x00,0x00,0x00,0x40,0x20,0x18,0x07,0x00,0x00,0x00,
0x40,0x40,0x80,0xF0,0x80,0x40,0x40,0x00,0x02,0x02,0x01,0x0F,0x01,0x02,0x02,0x00,
0x00,0x00,0x00,0xF0,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x1F,0x01,0x01,0x01,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xB0,0x70,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x80,0x60,0x18,0x04,0x00,0x60,0x18,0x06,0x01,0x00,0x00,0x00,
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x0F,0x10,0x20,0x20,0x10,0x0F,0x00,
0x00,0x10,0x10,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
0x00,0x70,0x08,0x08,0x08,0x88,0x70,0x00,0x00,0x30,0x28,0x24,0x22,0x21,0x30,0x00,
0x00,0x30,0x08,0x88,0x88,0x48,0x30,0x00,0x00,0x18,0x20,0x20,0x20,0x11,0x0E,0x00,
0x00,0x00,0xC0,0x20,0x10,0xF8,0x00,0x00,0x00,0x07,0x04,0x24,0x24,0x3F,0x24,0x00,
0x00,0xF8,0x08,0x88,0x88,0x08,0x08,0x00,0x00,0x19,0x21,0x20,0x20,0x11,0x0E,0x00,
0x00,0xE0,0x10,0x88,0x88,0x18,0x00,0x00,0x00,0x0F,0x11,0x20,0x20,0x11,0x0E,0x00,
0x00,0x38,0x08,0x08,0xC8,0x38,0x08,0x00,0x00,0x00,0x00,0x3F,0x00,0x00,0x00,0x00,
0x00,0x70,0x88,0x08,0x08,0x88,0x70,0x00,0x00,0x1C,0x22,0x21,0x21,0x22,0x1C,0x00,
0x00,0xE0,0x10,0x08,0x08,0x10,0xE0,0x00,0x00,0x00,0x31,0x22,0x22,0x11,0x0F,0x00,
0x00,0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00,
0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x60,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x40,0x20,0x10,0x08,0x00,0x00,0x01,0x02,0x04,0x08,0x10,0x20,0x00,
0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,
0x00,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,0x20,0x10,0x08,0x04,0x02,0x01,0x00,
0x00,0x70,0x48,0x08,0x08,0x08,0xF0,0x00,0x00,0x00,0x00,0x30,0x36,0x01,0x00,0x00,
0xC0,0x30,0xC8,0x28,0xE8,0x10,0xE0,0x00,0x07,0x18,0x27,0x24,0x23,0x14,0x0B,0x00,
0x00,0x00,0xC0,0x38,0xE0,0x00,0x00,0x00,0x20,0x3C,0x23,0x02,0x02,0x27,0x38,0x20,
0x08,0xF8,0x88,0x88,0x88,0x70,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x11,0x0E,0x00,
0xC0,0x30,0x08,0x08,0x08,0x08,0x38,0x00,0x07,0x18,0x20,0x20,0x20,0x10,0x08,0x00,
0x08,0xF8,0x08,0x08,0x08,0x10,0xE0,0x00,0x20,0x3F,0x20,0x20,0x20,0x10,0x0F,0x00,
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x20,0x23,0x20,0x18,0x00,
0x08,0xF8,0x88,0x88,0xE8,0x08,0x10,0x00,0x20,0x3F,0x20,0x00,0x03,0x00,0x00,0x00,
0xC0,0x30,0x08,0x08,0x08,0x38,0x00,0x00,0x07,0x18,0x20,0x20,0x22,0x1E,0x02,0x00,
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x20,0x3F,0x21,0x01,0x01,0x21,0x3F,0x20,
0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
0x00,0x00,0x08,0x08,0xF8,0x08,0x08,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,0x00,
0x08,0xF8,0x88,0xC0,0x28,0x18,0x08,0x00,0x20,0x3F,0x20,0x01,0x26,0x38,0x20,0x00,
0x08,0xF8,0x08,0x00,0x00,0x00,0x00,0x00,0x20,0x3F,0x20,0x20,0x20,0x20,0x30,0x00,
0x08,0xF8,0xF8,0x00,0xF8,0xF8,0x08,0x00,0x20,0x3F,0x00,0x3F,0x00,0x3F,0x20,0x00,
0x08,0xF8,0x30,0xC0,0x00,0x08,0xF8,0x08,0x20,0x3F,0x20,0x00,0x07,0x18,0x3F,0x00,
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x10,0x20,0x20,0x20,0x10,0x0F,0x00,
0x08,0xF8,0x08,0x08,0x08,0x08,0xF0,0x00,0x20,0x3F,0x21,0x01,0x01,0x01,0x00,0x00,
0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,0x0F,0x18,0x24,0x24,0x38,0x50,0x4F,0x00,
0x08,0xF8,0x88,0x88,0x88,0x88,0x70,0x00,0x20,0x3F,0x20,0x00,0x03,0x0C,0x30,0x20,
0x00,0x70,0x88,0x08,0x08,0x08,0x38,0x00,0x00,0x38,0x20,0x21,0x21,0x22,0x1C,0x00,
0x18,0x08,0x08,0xF8,0x08,0x08,0x18,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,
0x08,0xF8,0x08,0x00,0x00,0x08,0xF8,0x08,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
0x08,0x78,0x88,0x00,0x00,0xC8,0x38,0x08,0x00,0x00,0x07,0x38,0x0E,0x01,0x00,0x00,
0xF8,0x08,0x00,0xF8,0x00,0x08,0xF8,0x00,0x03,0x3C,0x07,0x00,0x07,0x3C,0x03,0x00,
0x08,0x18,0x68,0x80,0x80,0x68,0x18,0x08,0x20,0x30,0x2C,0x03,0x03,0x2C,0x30,0x20,
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x20,0x3F,0x20,0x00,0x00,0x00,
0x10,0x08,0x08,0x08,0xC8,0x38,0x08,0x00,0x20,0x38,0x26,0x21,0x20,0x20,0x18,0x00,
0x00,0x00,0x00,0xFE,0x02,0x02,0x02,0x00,0x00,0x00,0x00,0x7F,0x40,0x40,0x40,0x00,
0x00,0x0C,0x30,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x38,0xC0,0x00,
0x00,0x02,0x02,0x02,0xFE,0x00,0x00,0x00,0x00,0x40,0x40,0x40,0x7F,0x00,0x00,0x00,
0x00,0x00,0x04,0x02,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
0x00,0x02,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x19,0x24,0x22,0x22,0x22,0x3F,0x20,
0x08,0xF8,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x3F,0x11,0x20,0x20,0x11,0x0E,0x00,
0x00,0x00,0x00,0x80,0x80,0x80,0x00,0x00,0x00,0x0E,0x11,0x20,0x20,0x20,0x11,0x00,
0x00,0x00,0x00,0x80,0x80,0x88,0xF8,0x00,0x00,0x0E,0x11,0x20,0x20,0x10,0x3F,0x20,
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x22,0x22,0x22,0x22,0x13,0x00,
0x00,0x80,0x80,0xF0,0x88,0x88,0x88,0x18,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x6B,0x94,0x94,0x94,0x93,0x60,0x00,
0x08,0xF8,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
0x00,0x80,0x98,0x98,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
0x00,0x00,0x00,0x80,0x98,0x98,0x00,0x00,0x00,0xC0,0x80,0x80,0x80,0x7F,0x00,0x00,
0x08,0xF8,0x00,0x00,0x80,0x80,0x80,0x00,0x20,0x3F,0x24,0x02,0x2D,0x30,0x20,0x00,
0x00,0x08,0x08,0xF8,0x00,0x00,0x00,0x00,0x00,0x20,0x20,0x3F,0x20,0x20,0x00,0x00,
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x20,0x3F,0x20,0x00,0x3F,0x20,0x00,0x3F,
0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x3F,0x21,0x00,0x00,0x20,0x3F,0x20,
0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x1F,0x20,0x20,0x20,0x20,0x1F,0x00,
0x80,0x80,0x00,0x80,0x80,0x00,0x00,0x00,0x80,0xFF,0xA1,0x20,0x20,0x11,0x0E,0x00,
0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x00,0x00,0x0E,0x11,0x20,0x20,0xA0,0xFF,0x80,
0x80,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x20,0x20,0x3F,0x21,0x20,0x00,0x01,0x00,
0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x33,0x24,0x24,0x24,0x24,0x19,0x00,
0x00,0x80,0x80,0xE0,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x00,0x00,
0x80,0x80,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x1F,0x20,0x20,0x20,0x10,0x3F,0x20,
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x00,0x01,0x0E,0x30,0x08,0x06,0x01,0x00,
0x80,0x80,0x00,0x80,0x00,0x80,0x80,0x80,0x0F,0x30,0x0C,0x03,0x0C,0x30,0x0F,0x00,
0x00,0x80,0x80,0x00,0x80,0x80,0x80,0x00,0x00,0x20,0x31,0x2E,0x0E,0x31,0x20,0x00,
0x80,0x80,0x80,0x00,0x00,0x80,0x80,0x80,0x80,0x81,0x8E,0x70,0x18,0x06,0x01,0x00,
0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x21,0x30,0x2C,0x22,0x21,0x30,0x00,
0x00,0x00,0x00,0x00,0x80,0x7C,0x02,0x02,0x00,0x00,0x00,0x00,0x00,0x3F,0x40,0x40,
0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,
0x00,0x02,0x02,0x7C,0x80,0x00,0x00,0x00,0x00,0x40,0x40,0x3F,0x00,0x00,0x00,0x00,
0x00,0x06,0x01,0x01,0x02,0x02,0x04,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

/*******************************************************************************
**  函数名称  : void LCD_13264_GPIO_Configuration()
**  函数功能  : 初始化LCD13264的GPIO
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_13264_GPIO_Configuration()
{
	GPIO_InitTypeDef  GPIO_InitStructure;	
	
	//LCDON
	RCC_AHB1PeriphClockCmd(LCD_POWER_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_POWER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LCD_POWER_PORT, &GPIO_InitStructure);	
	LCD_POWER_ON;
	//LCD_RST
	RCC_AHB1PeriphClockCmd(LCD_RST_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
	GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);	
	//LCD_CS
	RCC_AHB1PeriphClockCmd(LCD_CS_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
	GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);	
	//LCD_A0
	RCC_AHB1PeriphClockCmd(LCD_A0_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_A0_PIN;
	GPIO_Init(LCD_A0_PORT, &GPIO_InitStructure);	
	//LCD_SCK
	RCC_AHB1PeriphClockCmd(LCD_SCK_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_SCK_PIN;
	GPIO_Init(LCD_SCK_PORT, &GPIO_InitStructure);	
	//LCD_MISO
	RCC_AHB1PeriphClockCmd(LCD_MISO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_MISO_PIN;
	GPIO_Init(LCD_MISO_PORT, &GPIO_InitStructure);		
	
}

/*******************************************************************************
**  函数名称  : u8 LCD_SPI_Send_Byte(u8 Byte)
**  函数功能  : LCD通过SPI发送字节
**  输    入  : 无	
**  输    出  : SPI返回的字节
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
u8 LCD_SPI_Send_Byte(u8 Byte)
{

	u8 ch = 2;
#if 0
	LCD_SPI_CS_LOW;
#if 1
	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(LCD_SPI, Byte);

	while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_RXNE) == RESET);

	ch	= SPI_I2S_ReceiveData(LCD_SPI);
#endif
	LCD_SPI_CS_HIGH;
#endif	
	return ch;

}

/*******************************************************************************
**  函数名称  :void Lcd_WriteData(u8 Byte)
**  函数功能  :给LCD发送数据
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Lcd_WriteData(u8 Byte);

/*******************************************************************************
**  函数名称  : void LCD_Send_Data(u8 Byte)
**  函数功能  : 给LCD发送数据
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Send_Data(u8 Byte)
{
#if 1
	u8 i;
	
	LCD_CS_LOW();
	LCD_A0_HIGH();
	for(i=0;i<8;i++)
	{
		if(0x01 == (0x01&Byte))
		{
			LCD_DA_HIGH();
		}
		else
		{
			LCD_DA_LOW();
		}
		LCD_SCK_LOW();
		//LCD_SCK_HIGH();
		LCD_SCK_HIGH();
		LCD_SCK_LOW();
		Byte=Byte >> 1;
	}
	LCD_A0_LOW();
	LCD_CS_HIGH();

#else
	LCD_CS_LOW;
	LCD_A0_HIGH;
	LCD_SPI_Send_Byte(Byte);
	LCD_CS_HIGH;
#endif
}

/*******************************************************************************
**  函数名称  : void LCD_Send_Cmd(u8 Byte)
**  函数功能  : 给LCD发送命令
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Send_Cmd(u8 Byte)
{
#if 1
	u8 i;

	LCD_CS_LOW();
	LCD_A0_LOW();
	for(i=0;i<8;i++)
	{
		if(0x80 == (0x80&Byte))
		{
		LCD_DA_HIGH();
		}
		else
		{
		LCD_DA_LOW();
		}
		LCD_SCK_LOW();
		LCD_SCK_HIGH();
		LCD_SCK_HIGH();
		LCD_SCK_LOW();
		Byte=Byte<<1;
	}
	LCD_A0_HIGH();
	LCD_CS_HIGH();

#else
	LCD_CS_LOW;
	LCD_A0_LOW;
	LCD_SPI_Send_Byte(Byte);
	LCD_CS_HIGH;
#endif
}

/*******************************************************************************
**  函数名称  : void LCD_Set_Column(u8 nColumn)
**  函数功能  : 设置LCD列
**  输    入  : nColumn:列
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Set_Column(u8 nColumn)
{
	LCD_Send_Cmd(0x10 | (nColumn >> 4));
	LCD_Send_Cmd((nColumn & 0x0F));
}

/*******************************************************************************
**  函数名称  : void LCD_Set_Page(u8 nPage)
**  函数功能  : 设置LCD页
**  输    入  : nPage:页
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Set_Page(u8 nPage)
{
	LCD_Send_Cmd(0xb0 | (nPage  & 0x0F));
}

/*******************************************************************************
**  函数名称  : void LCD_Set_Pos(u8 nPage,u8 nColumn)
**  函数功能  : 设置LCD的页和列
**  输    入  : nPage:设置的页()
				nColumn:设置的列
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Set_Pos(u8 nPage,u8 nColumn)
{
	LCD_Send_Cmd(0xb0 | (nPage  & 0x0F));
	LCD_Send_Cmd(0x10 | (nColumn >> 4));
	LCD_Send_Cmd((nColumn & 0x0F));
}

/*******************************************************************************
**  函数名称  : void LCD_Set_Mode(u8 bMode)
**  函数功能  : 设置LCD的显示模式
**  输    入  : bMode: 0 正常显示1:反显
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Set_Mode(u8 bMode)
{
	LCD_Send_Cmd(0xA6 | bMode);
}

/*******************************************************************************
**  函数名称  : bool LCD_Display_Ascii(u8 bMode,u8 nLine,u8 nColumn,u8 *pMsg,u8 nLen)
**  函数功能  : LCD显示ASCII码
**  输    入  : 	bMode: 0:正常显示 1:反显
					nLine:第几行显示(从上到下1-4)
					nColumn:从第几列开始显示(0-120)
					pMsg:指向要显示的数据
					nLen:数据长度(1-16)
**  输    出  :  true:成功 false:失败
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
LCD的行从上到下分4行。
LCD的列从左到右分132列。
*******************************************************************************/
bool LCD_Display_Ascii(u8 bMode,u8 nLine,u8 nColumn,u8 *pMsg,u8 nLen)
{
	u8 LineMapPage[5] = {0,3,1,7,5};
	u8 nPage = 0;
	u8 i = 0,j = 0;
	u8 AsciiCode[16] = "";
	//参数检测
	if(bMode > 1 
		|| (nLine == 0 || nLine > 4)
		|| (nColumn > 120)
		|| pMsg == NULL
		|| (nLen ==0 || nLen > 16))
	{
		printf("%s:Para Err\r\n",__FUNCTION__);
		return false;	
	}

	nPage = LineMapPage[nLine];
	LCD_Set_Pos(nPage,nColumn);
	for(j=0;j<nLen;j++)//显示字符串
	{
		memcpy(AsciiCode,&AsciiTab[pMsg[j]][0],sizeof(AsciiCode));
		if(bMode == 1)//若为反显则转换字模编码
		{
			for(i = 0;i<sizeof(AsciiCode);i++)
			{
				AsciiCode[i] = AsciiCode[i] ^ 0xFF;	
			}
		}
		//显示该字符上半部分
		LCD_Set_Pos(nPage,nColumn+8*j);
		for(i=0;i<8;i++)
		{
			Lcd_WriteData(AsciiCode[i]);		
		}
		//显示该字符下半部分	
		LCD_Set_Pos(nPage-1,nColumn+8*j);
		for(i=8;i<16;i++)
		{
			Lcd_WriteData(AsciiCode[i]);		
		}
	}

	return true;
}

/*******************************************************************************
**  函数名称  :void Lcd_WriteData(u8 Byte)
**  函数功能  :给LCD发送数据
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Lcd_WriteData(u8 Byte)
{
	LCD_Send_Data(Byte);
}
/*******************************************************************************
**  函数名称  : void Lcd_WriteCom(u8 Byte)
**  函数功能  : 给LCD发送命令
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void Lcd_WriteCom(u8 Byte)
{
	LCD_Send_Cmd(Byte);
}

/*******************************************************************************
**  函数名称  : void LCD_Home_Page()
**  函数功能  : 主界面
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Home_Page()
{
	u8 tempBuf[LCD_LINE_ASCII_LEN] = LCD_CLEAR_LINE;
	char *pTypeHead=NULL,*pTypeEnd = NULL;

	if(g_MenuIndex != 0)
		return;
	pTypeHead = strchr((char *)g_GnssCmdInfo.sGnssType,'\"');
	pTypeEnd = strchr(pTypeHead+1,'\"');
	if(pTypeEnd == NULL)
	{
		memcpy(tempBuf,"Gnss Type Is NUL",LCD_LINE_ASCII_LEN);
	}
	else
	{
		memcpy(tempBuf,pTypeHead + 1,pTypeEnd-pTypeHead - 1);
	}
	LCD_Display_Ascii(0,1,0,tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,2,0,"Fct_V0.1 Project",LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,3,0,"  Press Menu!   ",LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE	,LCD_LINE_ASCII_LEN); 
}

/*******************************************************************************
**  函数名称  : void LCD_Screen_Clear()
**  函数功能  : LCD清屏
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Screen_Clear()
{
	u8 nPage,nColumn;

	for(nPage=0;nPage<LCD_PAGE_MAX;nPage++)
	{
		Lcd_WriteCom(0x00);
		Lcd_WriteCom(0x10);
		Lcd_WriteCom(0xb0|nPage);
		for(nColumn=0;nColumn<LCD_COLUMN_MAX;nColumn++)
			{
				Lcd_WriteData(0x00);
			}
   }
}

void LCD_Delay(void)
{
    __IO uint32_t i = 0;
    for(i = 0xFF; i != 0; i--)
    {
    }
}

/*******************************************************************************
**  函数名称  : void LCD_Init()
**  函数功能  : LCD初始化
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Init()
{	
	Lcd_WriteCom(0xe2);  //0xE2=B1110 0010,软件复位
	LCD_Delay();

	Lcd_WriteCom(0xa2);  //0xA2=B1010 0010,设置LCD偏压比,0-1/9
	LCD_Delay();
	
	Lcd_WriteCom(0xa0);  //0xA0=B1010 0000,显示地址增减,0-常规,从左到右
	LCD_Delay();
	
	Lcd_WriteCom(0xc0);  //0xC0=B11000000,行扫描顺序选择,0-普通顺序
	LCD_Delay();
		
	Lcd_WriteCom(0x27);  //0x27=B0010 0111,内部电压值电阻设置,(20h-27h) 27h ,改进型一体机的屏,正常使用的
	
	LCD_Delay();
		
	Lcd_WriteCom(0x81);  //0x81=B1000 0001,设置内部电阻微调,设置内部电阻微调，以设置液晶电压，此两个指令需紧接着使用
	LCD_Delay();
		
	Lcd_WriteCom(0x32);  //0x32=B0011 0010,设置电压值(0~63级),改进型一体机的屏,正常使用的
	
	LCD_Delay();
	
	Lcd_WriteCom(0x2f);  //0x2F=B0010 1111,电源控制集,选择内部电压供应操作模式。
	LCD_Delay();
	
	Lcd_WriteCom(0x40);  //0x40=B0100 0000,显示初始行设置
	LCD_Delay();
	Lcd_WriteCom(0xb0);  //0xB0=B1011 0000,显示页地址，共4 位,页0
	LCD_Delay();
	Lcd_WriteCom(0xb7);  //0xB7=B10110111,显示页地址，共4 位,页7,总页数是0~7共八页
	LCD_Delay();
	
	Lcd_WriteCom(0x01);  //0x01=B0000 0001 设置列地址低4位,-1
	LCD_Delay();
	Lcd_WriteCom(0x10);  //0x01=B0001 0000 设置列地址高4位,-0,构成B0000 0001 =1列
	LCD_Delay();
	
	Lcd_WriteCom(0x00);  //0x00=B0000 0000 设置列地址低4位,-0
	LCD_Delay();
	Lcd_WriteCom(0x18);  //0x01=B0001 1000 设置列地址高4位,-1000,构成B1000 0000 = 128列
	LCD_Delay();
	Lcd_WriteCom(0xaf);  //0xAF=B1010 1111, 显示开
	LCD_Delay();
}

/******************************菜单↓******************************/
u8 g_MenuIndex = 0;		//菜单的索引号
u8 g_LcdMode = LCD_MENU_MODE;//LCD的显示模式	
u8 g_LcdDisplayBuf[LCD_DISPLAY_LINE_NUM][16] = {0};//LCD显示的数据的缓冲区
u8 g_LcdAutoErrBuf[LCD_DISPLAY_LINE_NUM*3][16] = {0};//自动测试错误记录缓冲区
u8 g_LcdAutoErrIndex = 0;//自动测试错误数据索引
u8 g_LcdDisplayIndex = 0;//LCD缓冲区的索引号
GNSS_TypeDef g_GnssType = GNSS_NONE;

#if 1
MenuItem MenuTab[MENU_ITEM_NUM] = {
{0	,1	,0	,0	,0	,"Enter Menu"	,MainMenuFun},
{1	,0	,2	,2	,1	,"Auto Test"	,AutoTestFun},
{2	,0	,1	,1	,3	,"CheckSelf"	,GeneralFun},
{3	,2	,6	,4	,7	,"Io Test"		,GeneralFun},
{4	,2	,3	,5	,4	,"Uartx Test"	,UartxTestFun},
{5	,2	,4	,6	,5	,"Can Test"		,CanTestFun},
{6	,2	,5	,3	,6	,"Volt Test"	,VoltTestFun},
{7	,3	,8	,8	,7	,"CheckSelf Io +",CheckSelfIoHighFun},
{8	,3	,7	,7	,8	,"CheckSelf Io -",CheckSelfIoLowFun}
};
#else
MenuItem MenuTab[MENU_ITEM_NUM] = {
{0	,1	,0	,0	,0	,"Enter Menu"	,MainMenuFun},
{1	,0	,8	,2	,9	,"CheckSelf"	,GeneralFun},
{2	,0	,1	,3	,13	,"Gnss Type"	,GeneralFun},
{3	,0	,2	,4	,16	,"Single Test"	,GeneralFun},
{4	,0	,3	,5	,4	,"Auto Test"	,AutoTestFun},
{5	,0	,4	,6	,5	,"Reserve1"		,GeneralFun},
{6	,0	,5	,7	,6	,"Reserve2"		,GeneralFun},
{7	,0	,6	,8	,7	,"Reserve3"		,GeneralFun},
{8	,0	,7	,1	,8	,"Reserve4"		,GeneralFun},
{9	,1	,12	,10	,36	,"Io Test"		,GeneralFun},
{10	,1	,9	,11	,10	,"Uartx Test"	,UartxTestFun},
{11	,1	,10	,12	,11	,"Can Test"		,CanTestFun},
{12	,1	,11	,9	,12	,"Volt Test"	,VoltTestFun},
{13	,2	,15	,14	,13	,"GNSS SHANTUI"	,GNSSTypeFun},
{14	,2	,13	,15	,14	,"GNSS E"		,GNSSTypeFun},
{15	,2	,14	,13	,15	,"GNSS C"		,GNSSTypeFun},
{16	,3	,35	,17	,16	,"EYE+DEBUG"	,SingleTestFun},
{17	,3	,16	,18	,17	,"EYE+SIN"		,SingleTestFun},
{18	,3	,17	,19	,18	,"EYE+SOUT"		,SingleTestFun},
{19	,3	,18	,20	,19	,"EYE+VIN"		,SingleTestFun},
{20	,3	,19	,21	,20	,"EYE+VOUT"		,SingleTestFun},
{21	,3	,20	,22	,21	,"EYE+COM"		,SingleTestFun},
{22	,3	,21	,23	,22	,"EYE+GNSS"		,SingleTestFun},
{23	,3	,22	,24	,23	,"EYE+MBCOM"	,SingleTestFun},
{24	,3	,23	,25	,24	,"EYE+CHECKSELF",SingleTestFun},
{25	,3	,24	,26	,25	,"EYE+GNSSANTE"	,SingleTestFun},
{26	,3	,25	,27	,26	,"EYE+POWER"	,SingleTestFun},
{27	,3	,26	,28	,27	,"EYE+BATTERY"	,SingleTestFun},
{28	,3	,27	,29	,28	,"EYE+PWM"		,SingleTestFun},
{29	,3	,28	,30	,29	,"EYE+COUNT"	,SingleTestFun},
{30	,3	,29	,31	,30	,"Reserve1"		,SingleTestFun},
{31	,3	,30	,32	,31	,"Reserve2"		,SingleTestFun},
{32	,3	,31	,33	,32	,"Reserve3"		,SingleTestFun},
{33	,3	,32	,34	,33	,"Reserve4"		,SingleTestFun},
{34	,3	,33	,35	,34	,"Reserve5"		,SingleTestFun},
{35	,3	,34	,16	,35	,"Reserve6"		,SingleTestFun},
{36	,9	,37	,37	,36	,"CheckSelf Io +",CheckSelfIoHighFun},
{37	,9	,36	,36	,37	,"CheckSelf Io -",CheckSelfIoLowFun}
};
#endif
u8 sGnssCmdBuf[GNSS_CMD_BUF_LEN] = "";
//山推单项测试命令列表
u8 *(pGnssShanTuiCmdTab[]) = {
"EYE+DEBUG\r",
"EYE+SIN=1,+,3\rEYE+SIN=1,-,3\rEYE+SIN=2,+,3\rEYE+SIN=2,-,3\r",
"EYE+SOUT=1,+,3\rEYE+SOUT=1,-,3\rEYE+SOUT=2,+,3\rEYE+SOUT=2,-,3\rEYE+SOUT=3,+,3\rEYE+SOUT=3,-,3\r",
"EYE+VIN=1,24,3\r",
"EYE+VOUT=1,24,3\r",
"EYE+COM=\"COM_TEST\",1,9\r",
"EYE+GNSS=3\r",
"EYE+MBCOM=\"AT\",6\r",
"EYE+CHECKSELF=6\r",
"EYE+GNSSANTE=0,3\rEYE+GNSSANTE=1,3\rEYE+GNSSANTE=2,3\r",
"EYE+POWER=8,1,3\rEYE+POWER=24,1,3\rEYE+POWER=40,1,3\r",
"EYE+BATTERY=0,1,3\rEYE+BATTERY=1,1,3\r",
"EYE+PWM=10,500,3,3\r",
"EYE+COUNT=9\r",
"\r",
"\r",
"\r",
"\r",
"\r",
"\r"};

//GNSS E的命令列表
u8 *(pGnssECmdTab[]) = {
"EYE+DEBUG\r",
"EYE+SIN=1,+,3\rEYE+SIN=1,-,3\r",
"EYE+SOUT=1,+,3\rEYE+SOUT=1,-,3\r",
"EYE+VIN=1,24,3\r",
"EYE+VOUT=1,24,3\r",
"EYE+COM=\"COM_TEST\",1,9\r",
"EYE+GNSS=3\r",
"EYE+MBCOM=\"AT\",6\r",
"EYE+CHECKSELF=6\r",
"EYE+GNSSANTE=0,3\rEYE+GNSSANTE=1,3\rEYE+GNSSANTE=2,3\r",
"EYE+POWER=8,3,3\rEYE+POWER=24,1,3\rEYE+POWER=40,1,3\r",
"EYE+BATTERY=0,1,3\rEYE+BATTERY=1,1,3\r",
"EYE+PWM=10,500,3,3\r",
"EYE+COUNT=9\r",
"\r",
"\r",
"\r",
"\r",
"\r",
"\r"};
//山推机自动测试返回的字符串
u8 *(pShanTuiGnssAutoStr[]) = {
"DEBUG           ",
"SIN(1,+)        ",
"SIN(1,-)        ",
"SIN(2,+)        ",
"SIN(2,-)        ",
"SOUT(1,+)	     ",
"SOUT(1,-)	     ",
"SOUT(2,+)	     ",
"SOUT(2,-)	     ",
"SOUT(3,+)	     ",
"SOUT(3,-)	     ",
"COM(1)          ",
"GNSS            ",
"MBCOM           ",
"CHECKSELF       ",
"GNSSANTE(0)     ",
"GNSSANTE(1)     ",
"GNSSANTE(2)     ",
"POWER(8)        ",
"POWER(24)       ",
"POWER(40)       ",
"BATTERY(0)      ",
"BATTERY(1)      ",
"COUNT           ",
};

//E型机自动测试返回的字符串
u8 *(pEGnssAutoStr[]) = {
"DEBUG           ",
"SIN(1,+)        ",
"SIN(1,-)        ",
"SOUT(1,+)	     ",
"SOUT(1,-)	     ",
"COM(1)          ",
"GNSS            ",
"MBCOM           ",
"CHECKSELF       ",
"GNSSANTE(0)     ",
"GNSSANTE(1)     ",
"GNSSANTE(2)     ",
"POWER(8)        ",
"POWER(24)       ",
"POWER(40)       ",
"BATTERY(0)      ",
"BATTERY(1)      ",
"COUNT           ",
};

/*******************************************************************************
**  函数名称  : void MainMenuFun()
**  函数功能  : 主界面
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void MainMenuFun()
{
    
	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	

	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],"Fct_V0.1 Project",LCD_LINE_ASCII_LEN);
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],"  Press Menu!  ",LCD_LINE_ASCII_LEN);
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
}

/*******************************************************************************
**  函数名称  : GNSSTypeFun()
**  函数功能  : 选择GNSS终端
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void GNSSTypeFun()
{
	u8 tempBuf[LCD_LINE_ASCII_LEN] = LCD_CLEAR_LINE;
	//被测终端类型
	if(g_MenuIndex == 13)
	{
		g_GnssType = GNSS_SHANTUI;
		memcpy(tempBuf,"Select:GNSS ST  ",16);
	}
	else if(g_MenuIndex == 14)
	{
		g_GnssType = GNSS_E;
		memcpy(tempBuf,"Select:GNSS E   ",16);
	}
	else if(g_MenuIndex == 15)
	{
		g_GnssType = GNSS_C;
		memcpy(tempBuf,"Select:GNSS C   ",16);
	}
	else
	{
		g_GnssType = GNSS_NONE;
		memcpy(tempBuf,"Select:NONE     ",16);
	}

	LCD_Display_Ascii(0,1,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,2,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,3,0,tempBuf,16);
	LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN); 
	Delay(1000);	
}
/*******************************************************************************
**  函数名称  : void VoltTestFun()
**  函数功能  : 输出电压自测
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void VoltTestFun()
{
	u8 Volt1 = 0,Volt2 = 0,nPos;
	u8 tempBuf[LCD_LINE_ASCII_LEN] = LCD_CLEAR_LINE;

	
	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));		
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],"Set:08V,08V     ",LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,2,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));	
	// 2路电压输出8V，后读取反馈的ADC的值进行校验
	g_HC595State |= 0x000C0000;
	HC595_Renew();	
	//XVO_DISABLE;
	XHV_Volt_Set(8);	
	Delay(1000);//这里需要等待ADC稳定后采集。
	Volt1 = (int)((double)ADC3ConvertedValue[0] /4096*3.3*4+0.5);
	Volt2 = (int)((double)ADC3ConvertedValue[1] /4096*3.3*4+0.5);
	g_HC595State &= ~0x000C0000;
	HC595_Renew();		
	//XVO_ENABLE;
	XHV_Volt_Set(0);
	Delay(100);

	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"RCV:",4);
	nPos = 4;
	{
		u8	AscHi,AscLo,nHex;
		nHex = Volt1 & 0xFF;
		HexToTwoASC(nHex,&AscHi,&AscLo);
		tempBuf[nPos++] = AscHi;
		tempBuf[nPos++] = AscLo;
		tempBuf[nPos++] = 'V';
		tempBuf[nPos++] = ',';
		nHex = Volt2 & 0xFF;
		HexToTwoASC(nHex,&AscHi,&AscLo);
		tempBuf[nPos++] = AscHi;
		tempBuf[nPos++] = AscLo;
		tempBuf[nPos++] = 'V';
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,3,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
	
	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"Result:",7);
	nPos = 7;
	if(Volt1 != 8 || Volt2 != 8)
	{
		memcpy(&tempBuf[nPos],"FAIL",strlen("FAIL"));	
	}
	else
	{
		memcpy(&tempBuf[nPos],"OK",strlen("OK"));	
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
}

/*******************************************************************************
**  函数名称  : void CanTestFun()
**  函数功能  : CAN1和CAN2通讯自测
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void CanTestFun()
{
	FIFOQueue *(pCanFifoTab[]) = {Get_Can1_Rx_Fifo(),Get_Can2_Rx_Fifo()};
	u8 i = 0,tempBuf[LCD_LINE_ASCII_LEN] = LCD_CLEAR_LINE;
	CanTxMsg TxMessage;
	ElemType sGet[20] = "";
	
	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));	

	CAN1_Configuration(CAN_Mode_LoopBack);//设置回环模式
	CAN2_Configuration(CAN_Mode_LoopBack);//设置回环模式
	//清空缓冲数据
	for(i=0;i<sizeof(pCanFifoTab)/sizeof(pCanFifoTab[0]);i++)
	{
		QueueInit(pCanFifoTab[i]);	
	}		
		
	TxMessage.StdId = 0;
	TxMessage.ExtId = 0x1234;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.DLC = 8;				
	memcpy(TxMessage.Data,"CAN_TEST",8);
	CAN_Transmit(CAN1, &TxMessage); 
	CAN_Transmit(CAN2, &TxMessage);
	Delay(100);
	//检验数据是否正确
	for(i=0;i<sizeof(pCanFifoTab)/sizeof(pCanFifoTab[0]);i++)
	{
		u8 j = 0;

		memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
		memcpy(tempBuf,"Can",4);
		tempBuf[4] = i+0x31;
		memset(sGet,0,sizeof(sGet));
		while((isQueueEmpty(pCanFifoTab[i]) == QUEUE_NOT_EMPTY) && j < 8)
		{
			QueueOut(pCanFifoTab[i],&sGet[j]);
			j++;
		}			
		if(memcmp(TxMessage.Data,sGet,8) != 0 || j == 0)//数据错误或数据为0都算失败
		{
			memcpy(&tempBuf[5],":Fail",5);
		}		
		else
		{
			memcpy(&tempBuf[5],":OK",3);
		}
		memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	}
	LCD_Show_Line(2,4,(u8 *)&g_LcdDisplayBuf[0][0],strlen((char *)g_LcdDisplayBuf),0);	
	//清空缓冲数据
	for(i=0;i<sizeof(pCanFifoTab)/sizeof(pCanFifoTab[0]);i++)
	{
		QueueInit(pCanFifoTab[i]);	
	}
	//设置CAN1和CAN2为正常
	CAN1_Configuration(CAN_Mode_Normal);
	CAN2_Configuration(CAN_Mode_Normal);		
	return ;
}

/*******************************************************************************
**  函数名称  : void UartxTestFun()
**  函数功能  : 串口1-6自测
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
1.测试前需要短接串口2,4,5,6的TX和RX，串口1和串口3的AB线互连
*******************************************************************************/
void UartxTestFun()
{
	ElemType sSend[10] = "CHECK_SELF",sGet[10] = "";
	u8 i = 0,tempBuf[LCD_LINE_ASCII_LEN] = LCD_CLEAR_LINE;
	FIFOQueue *(pUartxFifoTab[]) = {Get_Uart1_Rx_Fifo(),Get_Uart2_Rx_Fifo(),
		Get_Uart3_Rx_Fifo(),Get_Uart4_Rx_Fifo(),Get_Uart5_Rx_Fifo(),
		Get_Uart6_Rx_Fifo()};
	
	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));	
	//清空缓冲数据
	for(i=0;i<sizeof(pUartxFifoTab)/sizeof(pUartxFifoTab[0]);i++)
	{
		QueueInit(pUartxFifoTab[i]);	
	}	
	//发送数据
	for(i=0;i<sizeof(pUartxFifoTab)/sizeof(pUartxFifoTab[0]);i++)
	{
		Uartx_SendMsg((COM_TypeDef)i,sSend,sizeof(sSend));
	}
	Delay(100);
	//检验数据是否正确
	for(i=0;i<sizeof(pUartxFifoTab)/sizeof(pUartxFifoTab[0]);i++)
	{
		u8 j = 0;

		memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
		memcpy(tempBuf,"Uart",4);
		tempBuf[4] = i+0x31;
		memset(sGet,0,sizeof(sGet));
		while((isQueueEmpty(pUartxFifoTab[i]) == QUEUE_NOT_EMPTY) && j< 10)
		{
			QueueOut(pUartxFifoTab[i],&sGet[j]);
			j++;
		}			
		if(memcmp(sSend,sGet,j) != 0 || j == 0)//数据错误或数据为0都算失败
		{
			memcpy(&tempBuf[5],":Fail",5);
		}		
		else
		{
			memcpy(&tempBuf[5],":OK",3);
		}
		memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	}

	LCD_Show_Line(2,4,(u8 *)&g_LcdDisplayBuf[0][0],strlen((char *)g_LcdDisplayBuf),0);	
	//清空缓冲数据
	for(i=0;i<sizeof(pUartxFifoTab)/sizeof(pUartxFifoTab[0]);i++)
	{
		QueueInit(pUartxFifoTab[i]);	
	}

}

/*******************************************************************************
**  函数名称  : void AutoTestFun()
**  函数功能  : 自动测试
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void AutoTestFun()
{
	FIFOQueue *pFifo = Get_Uart4_Rx_Fifo();
	u32 nCmdLen = 0;
	u8 *pCmdHead=NULL,*pCmdEnd = NULL;

	//将第一包数据放入串口缓冲区
	pCmdHead = (u8 *)strchr((char *)&g_GnssCmdInfo.sCmdBuf,'#');
	pCmdEnd = (u8 *)strchr((char *)pCmdHead + 1,'#');
	if(pCmdEnd == NULL)
	{
		LCD_Display_Ascii(0,1,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
		LCD_Display_Ascii(0,2,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
		LCD_Display_Ascii(0,3,0,"GNSS CMD ERROR!",16);
		LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN); 
		Delay(2000);			
		return;
	}
	nCmdLen = pCmdEnd - pCmdHead -1;
	QueueInStr(pFifo,g_GnssCmdInfo.sCmdBuf+1,nCmdLen);
	g_GnssCmdInfo.nSendPacket=1;
	//LCD显示信息
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));
	LCD_Display_Ascii(0,2,0,"Waiting......",strlen("Waiting......"));
	LCD_Display_Ascii(0,3,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);	
	g_UartFlagGroup |= FLAG_GRP_UART4;
	g_LcdMode = LCD_AUTO_TEST_MODE;
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	g_LcdAutoErrIndex= 0;
	memset(g_LcdAutoErrBuf,0,sizeof(g_LcdAutoErrBuf));		
}

/*******************************************************************************
**  函数名称  : void SingleTestFun()
**  函数功能  : 单项测试
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void SingleTestFun()
{
#if 0
	FIFOQueue *pFifo = Get_Uart4_Rx_Fifo();
	u8 *pCmdStr = 0;

	if(g_GnssType == GNSS_E)
	{
		pCmdStr = pGnssECmdTab[g_MenuIndex-16];
	}
	else if(g_GnssType == GNSS_SHANTUI)
	{	
		pCmdStr = pGnssShanTuiCmdTab[g_MenuIndex-16];
	}
	else
	{
		LCD_Display_Ascii(0,1,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
		LCD_Display_Ascii(0,2,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
		LCD_Display_Ascii(0,3,0,"GNSS TYPE ERROR!",16);
		LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN); 
		Delay(2000);
		return;			
	}

	//LCD显示信息
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));
	LCD_Display_Ascii(0,2,0,"Waiting......",strlen("Waiting......"));
	LCD_Display_Ascii(0,3,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);	
	//模拟PC发送测试数据到串口缓冲区
	QueueInStr(pFifo,pCmdStr,strlen((char *)pCmdStr));
	g_UartFlagGroup |= FLAG_GRP_UART4;
	g_LcdMode = LCD_SINGLE_TEST_MODE;
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
#endif	
}

/*******************************************************************************
**  函数名称  : void CheckSelfIoHighFun()
**  函数功能  : IO正输入自测
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
1.测试前将端子拨到正输入状态
*******************************************************************************/
void CheckSelfIoHighFun()
{	
	u32 nIoState = 0x0003FFFF;
	u8 tempBuf[16] = LCD_CLEAR_LINE,nPos = 0,i;

	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],"Set:0x0003FFFF  ",LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,2,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
	
	XHV_Volt_Set(8);
	g_HC595State &=~(0x0003FFFF);
	for(i=0;i<18;i++)
	{
		g_HC595State |= 1<<i;//高电平输出自测
		HC595_Renew();	
        Delay(200);
		//if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_RESET)
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_RESET)
		{
			nIoState &= ~(1<<i);	
		}
		g_HC595State &= ~(1<<i);
	}	
	XHV_Volt_Set(0);

	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"RCV:0x",6);
	nPos = 6;
	for(i=0;i<4;i++)
	{
		u8 	AscHi,AscLo,nHex;
		nHex = (nIoState >> (3-i)*8) & 0xFF;
		HexToTwoASC(nHex,&AscHi,&AscLo);
		tempBuf[nPos++] = AscHi;
		tempBuf[nPos++] = AscLo;
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,3,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
	
	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"Result:",7);
	nPos = 7;
	if(nIoState != 0x0003FFFF)
	{
		memcpy(&tempBuf[nPos],"FAIL",strlen("FAIL"));	
	}
	else
	{
		memcpy(&tempBuf[nPos],"OK",strlen("OK"));	
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
		
}

/*******************************************************************************
**  函数名称  : void CheckSelfIoLowFun()
**  函数功能  : IO负输入自测
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
1.测试前将端子拨到负输入状态
*******************************************************************************/
void CheckSelfIoLowFun()
{	
	u32 nIoState = 0x00000000;
	u8 tempBuf[16] = LCD_CLEAR_LINE,nPos = 0,i;

	g_LcdMode = LCD_DISPLAY_MODE;	
	g_LcdDisplayIndex = 0;
	memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));	
	LCD_Display_Ascii(0,1,0,MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],"Set:0x00000000  ",LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,2,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
	
	XHV_Volt_Set(8);
	for(i=0;i<18;i++)
	{
		g_HC595State |= (1<<i);//低电平输出自测
		HC595_Renew();	
		Delay(200);
		if(GPIO_ReadInputDataBit(FeedBackPortTab[i],FeedBackPinTab[i]) == Bit_SET)
		{
			nIoState |= 1<<i;	
		}
		g_HC595State &= ~(1<<i);
	}	
	XHV_Volt_Set(0);

	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"RCV:0x",6);
	nPos = 6;
	for(i=0;i<4;i++)
	{
		u8	AscHi,AscLo,nHex;
		nHex = (nIoState >> (3-i)*8) & 0xFF;
		HexToTwoASC(nHex,&AscHi,&AscLo);
		tempBuf[nPos++] = AscHi;
		tempBuf[nPos++] = AscLo;
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,3,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
	
	memset(tempBuf,ASCII_SPACE,sizeof(tempBuf));
	memcpy(tempBuf,"Result:",7);
	nPos = 7;
	if(nIoState != 0x00000000)
	{
		memcpy(&tempBuf[nPos],"FAIL",strlen("FAIL"));	
	}
	else
	{
		memcpy(&tempBuf[nPos],"OK",strlen("OK"));	
	}
	memcpy(&g_LcdDisplayBuf[g_LcdDisplayIndex++][0],tempBuf,LCD_LINE_ASCII_LEN);
	LCD_Display_Ascii(0,4,0,&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0],strlen((char *)&g_LcdDisplayBuf[g_LcdDisplayIndex-1][0]));
		
}

/*******************************************************************************
**  函数名称  : void GeneralFun()
**  函数功能  : 通用菜单函数
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void GeneralFun()
{
	printf("Menu%d:%d,%d,%d,%d,%d,%s\r\n",g_MenuIndex,MenuTab[g_MenuIndex].CurIndex,
				MenuTab[g_MenuIndex].DownIndex,MenuTab[g_MenuIndex].UpIndex,
				MenuTab[g_MenuIndex].EnterIndex,MenuTab[g_MenuIndex].BackIndex,
				MenuTab[g_MenuIndex].pMenuName);
	
}

/*******************************************************************************
**  函数名称  : void LCD_Show_Auto()
**  函数功能  : 自动测试的LCD显示
**  输    入  : 无	
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Show_Auto()
{
	u8 i = 0,index = 0,bMode = 0;

	index = g_LcdDisplayIndex;
	for(i=4;i>1;i--)
	{
		if(index == 0)
		{
			LCD_Display_Ascii(0,i,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
			continue;
		}
		if(i == 4)//第4行背显表示当前测试项
			bMode = 1;
		else
			bMode = 0;
		LCD_Display_Ascii(bMode,i,0,&g_LcdDisplayBuf[index-1][0],16);
		index--;
	}
}

/*******************************************************************************
**  函数名称  : void LCD_Show_Line(u8 nStartLine,u8 nEndLine,u8 *pMsgBuf,u16 nMsgLen,u8 nMsgIndex)
**  函数功能  : LCD数据显示
**  输    入  :  nStartLine:从第几行开始显示
				  nEndLine:显示到第几行
				  pMsgBuf:指向要显示的数据
				  nMsgLen:数据的长度
				  nMsgIndex:数据的索引号
**  输    出  : 无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Show_Line(u8 nStartLine,u8 nEndLine,u8 *pMsgBuf,u16 nMsgLen,u8 nMsgIndex)
{
	u8 i = 0;
	if((nStartLine == 0 || nStartLine >4)
		|| (nEndLine == 0 || nEndLine >4 || nEndLine <= nStartLine)
		|| (pMsgBuf == NULL)
		|| (nMsgLen == 0))
	{
		printf("%s:Para Err !\r\n",__FUNCTION__);
		return;
	}
	for(i=nStartLine;i<=nEndLine;i++)
	{
		if(nMsgLen <= 0)
		{
			LCD_Display_Ascii(0,i,0,LCD_CLEAR_LINE,LCD_LINE_ASCII_LEN);
			continue;
		}
		LCD_Display_Ascii(0,i,0,&pMsgBuf[nMsgIndex*16],16);
		nMsgIndex++;
		nMsgLen -= 16;
	}
}

/*******************************************************************************
**  函数名称  : void LCD_Show_Menu()
**  函数功能  : LCD菜单显示
**  输    入  :  无
**  输    出  :  无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
*******************************************************************************/
void LCD_Show_Menu()
{
	u8 nIndex = 0,i = 0;

	if(g_MenuIndex == 0)//主菜单
		return;
	LCD_Screen_Clear();			//清LCD屏
	LCD_Display_Ascii(1,1,0,(u8 *)MenuTab[g_MenuIndex].pMenuName,strlen((char *)MenuTab[g_MenuIndex].pMenuName));
	nIndex = MenuTab[g_MenuIndex].DownIndex;
	for(i = 2;i<5;i++)
	{
		if(nIndex == g_MenuIndex)//无后续菜单，停止显示
		{
			break;
		}
		else//显示菜单项
		{
			LCD_Display_Ascii(0,i,0,(u8 *)MenuTab[nIndex].pMenuName,strlen((char *)MenuTab[nIndex].pMenuName));	
			nIndex =MenuTab[nIndex].DownIndex;
		}
	}	
}

/*******************************************************************************
**  函数名称  : void LCD_Key_Hanle()
**  函数功能  : LCD按键处理
**  输    入  :  无
**  输    出  :  无
**  全局变量  : 无
**  调用函数  : 无
**  中断资源  : 无
**  备    注  :
键值:           按键1:680 按键2:1380 按键3:2040 按键4 2730
*******************************************************************************/
void LCD_Key_Hanle()
{	
	u16 ADC_KEY = ADC3ConvertedValue[6];
	static u8 nDisplayIndex;
	
	//显示内容
	if(g_LcdMode == LCD_SINGLE_TEST_MODE || g_LcdMode == LCD_DISPLAY_MODE)
	{
		switch(ADC_KEY/600)
		{
			case LCD_BACK:
				nDisplayIndex = 0;
				g_LcdMode = LCD_MENU_MODE;
				memset(g_LcdDisplayBuf,0,sizeof(g_LcdDisplayBuf));
				g_LcdDisplayIndex = 0;
				LCD_Show_Menu();
				break;
			case LCD_UP:
				if(nDisplayIndex > 0)
					nDisplayIndex--;
				LCD_Show_Line(2,4,(u8 *)&g_LcdDisplayBuf[0][0],strlen((char *)g_LcdDisplayBuf),nDisplayIndex);
				break;
			case LCD_DOWN:
				if(nDisplayIndex < (g_LcdDisplayIndex - 3))
					nDisplayIndex++;
				LCD_Show_Line(2,4,(u8 *)&g_LcdDisplayBuf[0][0],strlen((char *)g_LcdDisplayBuf),nDisplayIndex);					
				break;
			case LCD_ENTER://打印机打印信息
				
				break;
			default:
				return;
				break;
		}		
		return;	
	}
	//显示自动测试的错误结果
	if(g_LcdMode == LCD_AUTO_TEST_MODE )
	{
		switch(ADC_KEY/600)
		{
			case LCD_BACK:
				nDisplayIndex = 0;
				g_LcdMode = LCD_MENU_MODE;
				memset(g_LcdAutoErrBuf,0,sizeof(g_LcdAutoErrBuf));
				g_LcdAutoErrIndex= 0;
				LCD_Show_Menu();
				break;
			case LCD_UP:
				if(g_LcdAutoErrIndex > 0)
				{
					u8 tempBuf[LCD_LINE_ASCII_LEN] = "Fail Item:   /  ";
					if(nDisplayIndex > 2)
						nDisplayIndex -= 3;
					tempBuf[11] = (nDisplayIndex/3 + 1)/10 + '0';
					tempBuf[12] = (nDisplayIndex/3 + 1)%10 + '0';
					tempBuf[14] = g_LcdAutoErrIndex/10 + '0';
					tempBuf[15] = g_LcdAutoErrIndex%10 + '0';
					LCD_Display_Ascii(0,1,0,tempBuf,LCD_LINE_ASCII_LEN);
					LCD_Show_Line(2,4,(u8 *)&g_LcdAutoErrBuf[0][0],strlen((char *)g_LcdAutoErrBuf),nDisplayIndex);
				}
				break;
			case LCD_DOWN:
				if(g_LcdAutoErrIndex > 0)
				{
					u8 tempBuf[LCD_LINE_ASCII_LEN] = "Fail Item:   /  ";
					if(nDisplayIndex < (g_LcdAutoErrIndex*3 - 3))
						nDisplayIndex += 3;
					tempBuf[11] = (nDisplayIndex/3 + 1)/10 + '0';
					tempBuf[12] = (nDisplayIndex/3 + 1)%10 + '0';
					tempBuf[14] = g_LcdAutoErrIndex/10 + '0';
					tempBuf[15] = g_LcdAutoErrIndex%10 + '0';
					LCD_Display_Ascii(0,1,0,tempBuf,LCD_LINE_ASCII_LEN);
					LCD_Show_Line(2,4,(u8 *)&g_LcdAutoErrBuf[0][0],strlen((char *)g_LcdAutoErrBuf),nDisplayIndex);					
				}
				break;
			case LCD_ENTER://打印机打印信息
				
				break;
			default:
				return;
				break;
		}		
		return;	
	}

	//显示菜单
	switch(ADC_KEY/600)
	{
		case LCD_BACK:
			g_MenuIndex = MenuTab[g_MenuIndex].BackIndex;
			if(g_MenuIndex == 0)
			{
				LCD_Home_Page();
			}
			break;
		case LCD_UP:
			g_MenuIndex = MenuTab[g_MenuIndex].UpIndex;
			break;
		case LCD_DOWN:
			g_MenuIndex = MenuTab[g_MenuIndex].DownIndex;
			break;
		case LCD_ENTER:
			MenuTab[g_MenuIndex].HanleFun();
			g_MenuIndex = MenuTab[g_MenuIndex].EnterIndex;
			if(g_LcdMode != LCD_MENU_MODE)
				return;
			break;
		default:
			return;
			break;
	}
	LCD_Show_Menu();
}

/******************************菜单↑******************************/

