#include "ds3231.h"

extern I2C_HandleTypeDef hi2c1;

#define DS3231_ADDR  0x68

static uint8_t BCD2DEC(uint8_t dat)
{
    return (dat >> 4) * 10 + (dat & 0x0F);
}

static uint8_t DEC2BCD(uint8_t dat)
{
    return ((dat / 10) << 4) | (dat % 10);
}

void DS3231_Init(void)
{
    uint8_t buf[2] = {0x0E, 0x00};
    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR << 1, buf, 2, 100);
}

void DS3231_ReadTime(uint8_t *hour, uint8_t *min, uint8_t *sec)
{
    uint8_t reg = 0x00;
    uint8_t data[3];

    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR << 1, &reg, 1, 100);
    HAL_I2C_Master_Receive(&hi2c1, DS3231_ADDR << 1, data, 3, 100);

    *sec  = BCD2DEC(data[0] & 0x7F);
    *min  = BCD2DEC(data[1] & 0x7F);
    *hour = BCD2DEC(data[2] & 0x3F);
}

void DS3231_SetTime(uint8_t hour, uint8_t min, uint8_t sec)
{
    uint8_t buf[4];

    buf[0] = 0x00;              // ?????:?
    buf[1] = DEC2BCD(sec);      // ?
    buf[2] = DEC2BCD(min);      // ?
    buf[3] = DEC2BCD(hour);     // ?(24???)

    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR << 1, buf, 4, 100);
}

void DS3231_SetDate(uint8_t year, uint8_t month, uint8_t date, uint8_t weekday)
{
    uint8_t buf[5];

    buf[0] = 0x03;              // ?????:??
    buf[1] = DEC2BCD(weekday);  // 1~7
    buf[2] = DEC2BCD(date);     // ?
    buf[3] = DEC2BCD(month);    // ?
    buf[4] = DEC2BCD(year);     // ?(00~99)

    HAL_I2C_Master_Transmit(&hi2c1, DS3231_ADDR << 1, buf, 5, 100);
}