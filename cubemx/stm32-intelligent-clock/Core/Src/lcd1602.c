#include "lcd1602.h"

#define LCD_RS_GPIO_Port GPIOB
#define LCD_RS_Pin       GPIO_PIN_0
#define LCD_EN_GPIO_Port GPIOB
#define LCD_EN_Pin       GPIO_PIN_1
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D4_Pin       GPIO_PIN_10
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D5_Pin       GPIO_PIN_11
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D6_Pin       GPIO_PIN_12
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D7_Pin       GPIO_PIN_13

static void LCD_Delay(void)
{
    HAL_Delay(2);
}

static void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    LCD_Delay();
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    LCD_Delay();
}

static void LCD_WriteNibble(uint8_t nibble)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (nibble & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (nibble & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (nibble & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (nibble & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_EnablePulse();
}

static void LCD_WriteByte(uint8_t data, uint8_t rs)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, rs ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_WriteNibble(data >> 4);
    LCD_WriteNibble(data & 0x0F);
}

static void LCD_WriteCmd(uint8_t cmd)
{
    LCD_WriteByte(cmd, 0);
}

static void LCD_WriteData(uint8_t data)
{
    LCD_WriteByte(data, 1);
}

void LCD1602_Init(void)
{
    HAL_Delay(100);

    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);

    LCD_WriteNibble(0x03);
    HAL_Delay(10);
    LCD_WriteNibble(0x03);
    HAL_Delay(10);
    LCD_WriteNibble(0x03);
    HAL_Delay(10);
    LCD_WriteNibble(0x02);
    HAL_Delay(10);

    LCD_WriteCmd(0x28);
    HAL_Delay(5);
    LCD_WriteCmd(0x0C);
    HAL_Delay(5);
    LCD_WriteCmd(0x06);
    HAL_Delay(5);
    LCD_WriteCmd(0x01);
    HAL_Delay(10);
}

void LCD1602_Clear(void)
{
    LCD_WriteCmd(0x01);
    HAL_Delay(10);
}

void LCD1602_ShowString(uint8_t x, uint8_t y, char *str)
{
    uint8_t addr;

    if (y == 0)
    {
        addr = 0x80 + x;
    }
    else
    {
        addr = 0xC0 + x;
    }

    LCD_WriteCmd(addr);

    while (*str)
    {
        LCD_WriteData((uint8_t)*str);
        str++;
    }
}