// 16x2 HD44780 LCD driver, 4-bit mode, all lines on Port0
#include <lpc21xx.h>
#include "types.h"
#include "defines.h"
#include "delay.h"
#include "Elevator_defines.h"
#include "lcd.h"

static void LCD_Pulse(void)
{
	SETBIT(IOSET0, LCD_EN);
	delay_us(50);
	SETBIT(IOCLR0, LCD_EN);
	delay_us(50);
}

static void LCD_WriteNibble(u8 nibble)
{
	WRITEBIT(IOCLR0, LCD_D4, 1);
	WRITEBIT(IOCLR0, LCD_D5, 1);
	WRITEBIT(IOCLR0, LCD_D6, 1);
	WRITEBIT(IOCLR0, LCD_D7, 1);

	if(READBIT(nibble,0)) SETBIT(IOSET0, LCD_D4);
	if(READBIT(nibble,1)) SETBIT(IOSET0, LCD_D5);
	if(READBIT(nibble,2)) SETBIT(IOSET0, LCD_D6);
	if(READBIT(nibble,3)) SETBIT(IOSET0, LCD_D7);

	LCD_Pulse();
}

void LCD_Command(u8 cmd)
{
	WRITEBIT(IOCLR0, LCD_RS, 1);       // RS = 0 -> command
	LCD_WriteNibble((cmd>>4)&0x0F);
	LCD_WriteNibble(cmd&0x0F);
	if(cmd==0x01 || cmd==0x02) delay_ms(2); // clear/home need extra time
	else delay_us(50);
}

void LCD_Char(u8 data)
{
	SETBIT(IOSET0, LCD_RS);            // RS = 1 -> data
	LCD_WriteNibble((data>>4)&0x0F);
	LCD_WriteNibble(data&0x0F);
	delay_us(50);
}

void LCD_String(const s8 *str)
{
	while(*str)
	{
		LCD_Char((u8)*str);
		str++;
	}
}

void LCD_SetCursor(u8 row, u8 col)
{
	u8 addr = (row==0) ? (0x80+col) : (0xC0+col);
	LCD_Command(addr);
}

void LCD_Clear(void)
{
	LCD_Command(0x01);
}

void LCD_Init(void)
{
	IODIR0 |= (1<<LCD_RS)|(1<<LCD_EN)|(1<<LCD_D4)|(1<<LCD_D5)|(1<<LCD_D6)|(1<<LCD_D7);

	delay_ms(20);          // wait for LCD power-up
	LCD_WriteNibble(0x03);
	delay_ms(5);
	LCD_WriteNibble(0x03);
	delay_us(150);
	LCD_WriteNibble(0x03);
	LCD_WriteNibble(0x02); // set to 4-bit mode

	LCD_Command(0x28);     // 4-bit, 2 line, 5x8 font
	LCD_Command(0x0C);     // display ON, cursor OFF
	LCD_Command(0x06);     // entry mode: increment
	LCD_Command(0x01);     // clear display
	delay_ms(2);
}
