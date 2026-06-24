#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY_NONE  0
#define KEY_SET   1
#define KEY_UP    2
#define KEY_DOWN  3
#define KEY_MODE  4

uint8_t KEY_Scan(void);
void KEY_1msScan(void);

#endif