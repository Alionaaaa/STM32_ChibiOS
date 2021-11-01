#include "ch.h"
#include "hal.h"

// �������������� ��������� ������������, ������ ������ �������
static const SerialConfig uart_conf = {
  .speed = 115200,
  .cr1 = 0,
  .cr2 = 0,
  .cr3 = 0
};

// ������ ��������� �� ������� � ����������
SerialDriver *uart3 = &SD3;

int main(void) {
    halInit();
    chSysInit();
// ��������� ������� � ������
    sdStart(uart3, &uart_conf);
// ��������� ���� � ��������� Rx, Tx
    palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
    palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );
// ���������� ����� ���� float �� 10 ���������
    float buf[10] = {1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5};
    while (true) {
// ��� � ������� ���������� ���� ����� �� ����������������� �����. �������� ��������, ��� ������ ���������� � uint8_t *, � ���������� ������ ������������� � 4 ����, ��� ��� float ��� 4 �����.
        sdWrite(uart3, (uint8_t *)buf, 40);
        chThdSleepMilliseconds(1000);
    }
}
