#ifndef __LCD1602_H
#define __LCD1602_H

#include "main.h"

void LCD1602_Init(void);
void LCD1602_Clear(void);
void LCD1602_ShowString(uint8_t x, uint8_t y, char* str);

#endif