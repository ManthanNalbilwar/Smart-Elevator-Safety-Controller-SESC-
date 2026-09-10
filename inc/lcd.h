#ifndef LCD_H
#define LCD_H
#include "types.h"

void LCD_Init(void);
void LCD_Command(u8 cmd);
void LCD_Char(u8 data);
void LCD_String(const s8 *str);
void LCD_SetCursor(u8 row, u8 col);
void LCD_Clear(void);

#endif
