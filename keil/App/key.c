#include "key.h"

static uint16_t key_cnt = 0;
static uint8_t key_state = 0;

void KEY_1msScan(void)
{
    if (key_cnt < 20) key_cnt++;
}

uint8_t KEY_Scan(void)
{
    if (key_cnt < 20) return KEY_NONE;
    key_cnt = 0;

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) return KEY_SET;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) return KEY_UP;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET) return KEY_DOWN;
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) return KEY_MODE;

    return KEY_NONE;
}