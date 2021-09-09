#include "ch.h"
#include "hal.h"

// ������ ������������ ��������, ������� ������������ i2c 54���
static const I2CConfig i2c1_conf = {
    .timingr = STM32_TIMINGR_PRESC(14U)  |
    STM32_TIMINGR_SCLDEL(3U)  | STM32_TIMINGR_SDADEL(2U) |
    STM32_TIMINGR_SCLH(14U)   | STM32_TIMINGR_SCLL(17U),
    .cr1 = 0,
    .cr2 = 0
};

// ������� ��������� �� ������� � ����������
static I2CDriver* i2c1 =  &I2CD1;

int main(void) {
    halInit();
    chSysInit();
    // �������� ��������
    i2cStart(i2c1, &i2c1_conf);
    // ��������� ���� B8 � B9 � ����� i2c1
    palSetLineMode(PAL_LINE(GPIOB, 8), PAL_MODE_ALTERNATE(4));
    palSetLineMode(PAL_LINE(GPIOB, 9), PAL_MODE_ALTERNATE(4));
    // �������� ������� ��� ������ � ������ ������
    uint8_t txbuf[1] = {0x0F};
    uint8_t rxbuf[1] = {0};
    // ��������� � ���������� � ������� 1101010 �������� �� �������� �� ������ 0x0F (������� WHO_AM_I ��������� l3gd20h)
    msg_t msg = i2cMasterTransmitTimeout(i2c1, 0b1101010, txbuf, 1, rxbuf, 1, 1000);
    // ���� ���������� �������� ������������ ����������, ������ ������ ���������
    if (rxbuf[0] == 0b11010111) palSetLine(LINE_LED1);
    // �������� ������� ������
    i2cflags_t flag = i2cGetErrors(i2c1);
    // ���� �� ������ ACK ���, ������ ������ ���������
    if (flag == I2C_ACK_FAILURE) palSetLine(LINE_LED2);
    while (true) {
        chThdSleepMilliseconds(1000);
    }
}
