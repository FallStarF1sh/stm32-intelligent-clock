#include "main.h"
#include "gpio.h"
#include "lcd1602.h"
#include <stdio.h>

void SystemClock_Config(void);
static void LCD_GPIO_ForceInit(void);
static void KEY_GPIO_ForceInit(void);
static void BUZZER_GPIO_ForceInit(void);
static uint8_t Key_Read(void);
static void Time_AddOneSecond(void);
static void Time_Show(void);
static void Alarm_Check(void);
static void Alarm_Stop(void);

#define KEY_NONE  0
#define KEY_SET   1
#define KEY_UP    2
#define KEY_DOWN  3
#define KEY_MODE  4

uint8_t hour = 12;
uint8_t min = 0;
uint8_t sec = 55;

uint8_t alarm_hour = 12;
uint8_t alarm_min = 01;
uint8_t alarm_enable = 1;
uint8_t alarm_ringing = 0;

uint8_t set_mode = 0;
uint32_t last_tick = 0;
uint32_t buzzer_tick = 0;
uint8_t buzzer_state = 0;

char lcd_buf[17];

int main(void)
{
  HAL_Init();

  MX_GPIO_Init();

  LCD_GPIO_ForceInit();
  KEY_GPIO_ForceInit();
  BUZZER_GPIO_ForceInit();

  LCD1602_Init();
  LCD1602_Clear();

  last_tick = HAL_GetTick();
  buzzer_tick = HAL_GetTick();

  while (1)
  {
    uint8_t key = Key_Read();

    if (key == KEY_SET)
    {
      set_mode++;
      if (set_mode > 5)
      {
        set_mode = 0;
      }
      Alarm_Stop();
    }
    else if (key == KEY_MODE)
    {
      alarm_enable = !alarm_enable;
      Alarm_Stop();
    }
    else if (key == KEY_UP)
    {
      if (set_mode == 1)
      {
        hour = (hour + 1) % 24;
      }
      else if (set_mode == 2)
      {
        min = (min + 1) % 60;
      }
      else if (set_mode == 3)
      {
        sec = (sec + 1) % 60;
      }
      else if (set_mode == 4)
      {
        alarm_hour = (alarm_hour + 1) % 24;
      }
      else if (set_mode == 5)
      {
        alarm_min = (alarm_min + 1) % 60;
      }
      Alarm_Stop();
    }
    else if (key == KEY_DOWN)
    {
      if (set_mode == 1)
      {
        hour = (hour == 0) ? 23 : hour - 1;
      }
      else if (set_mode == 2)
      {
        min = (min == 0) ? 59 : min - 1;
      }
      else if (set_mode == 3)
      {
        sec = (sec == 0) ? 59 : sec - 1;
      }
      else if (set_mode == 4)
      {
        alarm_hour = (alarm_hour == 0) ? 23 : alarm_hour - 1;
      }
      else if (set_mode == 5)
      {
        alarm_min = (alarm_min == 0) ? 59 : alarm_min - 1;
      }
      Alarm_Stop();
    }

    if (set_mode == 0 && HAL_GetTick() - last_tick >= 1000)
    {
      last_tick += 1000;
      Time_AddOneSecond();
      Alarm_Check();
    }

    if (alarm_ringing)
    {
      if (HAL_GetTick() - buzzer_tick >= 250)
      {
        buzzer_tick = HAL_GetTick();
        buzzer_state = !buzzer_state;
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, buzzer_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
      }
    }

    Time_Show();
    HAL_Delay(100);
  }
}

static uint8_t Key_Read(void)
{
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
      while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET);
      return KEY_SET;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
      while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET);
      return KEY_UP;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET)
    {
      while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET);
      return KEY_DOWN;
    }
  }

  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET)
  {
    HAL_Delay(20);
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET)
    {
      while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET);
      return KEY_MODE;
    }
  }

  return KEY_NONE;
}

static void Time_AddOneSecond(void)
{
  sec++;
  if (sec >= 60)
  {
    sec = 0;
    min++;
  }

  if (min >= 60)
  {
    min = 0;
    hour++;
  }

  if (hour >= 24)
  {
    hour = 0;
  }
}

static void Alarm_Check(void)
{
  if (alarm_enable && hour == alarm_hour && min == alarm_min && sec == 0)
  {
    alarm_ringing = 1;
  }
}

static void Alarm_Stop(void)
{
  alarm_ringing = 0;
  buzzer_state = 0;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

static void Time_Show(void)
{
  sprintf(lcd_buf, "%02d:%02d:%02d %s", hour, min, sec, alarm_enable ? "ON " : "OFF");
  LCD1602_ShowString(0, 0, lcd_buf);

  sprintf(lcd_buf, "A %02d:%02d ", alarm_hour, alarm_min);
  LCD1602_ShowString(0, 1, lcd_buf);

  if (set_mode == 0)
  {
    LCD1602_ShowString(9, 1, "RUN ");
  }
  else if (set_mode == 1)
  {
    LCD1602_ShowString(9, 1, "SET H");
  }
  else if (set_mode == 2)
  {
    LCD1602_ShowString(9, 1, "SET M");
  }
  else if (set_mode == 3)
  {
    LCD1602_ShowString(9, 1, "SET S");
  }
  else if (set_mode == 4)
  {
    LCD1602_ShowString(9, 1, "AL H ");
  }
  else if (set_mode == 5)
  {
    LCD1602_ShowString(9, 1, "AL M ");
  }
}

static void LCD_GPIO_ForceInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void KEY_GPIO_ForceInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void BUZZER_GPIO_ForceInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}