#ifndef __DS3231_H
#define __DS3231_H

#include "main.h"

void DS3231_Init(void);
void DS3231_ReadTime(uint8_t *hour, uint8_t *min, uint8_t *sec);

void DS3231_SetTime(uint8_t hour, uint8_t min, uint8_t sec);
void DS3231_SetDate(uint8_t year, uint8_t month, uint8_t date, uint8_t weekday);

#endif