#include "lcd1602.h"

#define LCD_RS_GPIO_Port GPIOB
#define LCD_RS_Pin       GPIO_PIN_0
#define LCD_EN_GPIO_Port GPIOB
#define LCD_EN_Pin       GPIO_PIN_1
#define LCD_D4_GPIO_Port  GPIOB
#define LCD_D4_Pin        GPIO_PIN_10
#define LCD_D5_GPIO_Port  GPIOB
#define LCD_D5_Pin        GPIO_PIN_11
#define LCD_D6_GPIO_Port  GPIOB
#define LCD_D6_Pin        GPIO_PIN_12
#define LCD_D7_GPIO_Port  GPIOB
#define LCD_D7_Pin        GPIO_PIN_13

static void LCD_Write4Bit(uint8_t data);
static void LCD_WriteCmd(uint8_t cmd);
static void LCD_WriteData(uint8_t data);
static void LCD_Enable(void);

void LCD1602_Init(void)
{
    HAL_Delay(15);
    LCD_Write4Bit(0x03 << 4);
    HAL_Delay(5);
    LCD_Write4Bit(0x03 << 4);
    HAL_Delay(5);
    LCD_Write4Bit(0x03 << 4);
    HAL_Delay(5);
    LCD_Write4Bit(0x02 << 4);

    LCD_WriteCmd(0x28);
    LCD_WriteCmd(0x0C);
    LCD_WriteCmd(0x06);
    LCD_WriteCmd(0x01);
    HAL_Delay(2);
}

void LCD1602_Clear(void)
{
    LCD_WriteCmd(0x01);
    HAL_Delay(2);
}

void LCD1602_ShowString(uint8_t x, uint8_t y, char* str)
{
    uint8_t addr = (y == 0) ? 0x80 + x : 0xC0 + x;
    LCD_WriteCmd(addr);
    while (*str) LCD_WriteData(*str++);
}

static void LCD_Write4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    LCD_Enable();
}

static void LCD_WriteCmd(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET);
    LCD_Write4Bit(cmd & 0xF0);
    LCD_Write4Bit((cmd << 4) & 0xF0);
}

static void LCD_WriteData(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET);
    LCD_Write4Bit(data & 0xF0);
    LCD_Write4Bit((data << 4) & 0xF0);
}

static void LCD_Enable(void)
{
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}