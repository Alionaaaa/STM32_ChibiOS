#include "ch.h"
#include "hal.h"

// callback �������, ������� ������ ��������� �� ������������ �������
void palcb_button(void* args){
    // �������������� ��������� � ���������� ����, � ������� ������ � uint8_t
    uint8_t arg = *((uint8_t*) args);
    // ��������, ��� �������� ��������� ��������
    if (arg == 5) palToggleLine(LINE_LED3);
}

int main(void) {
    halInit();
    chSysInit();
    // ������ �����, ������� ��������� � �������-����������
    uint8_t arg = 5;
    // ����������� ���� � ��������� �� �����
    palSetLineMode(LINE_LED3, PAL_MODE_OUTPUT_PUSHPULL);
    // � ���� � ������� �� ����
    palSetLineMode(GPIOC_BUTTON, PAL_MODE_INPUT_PULLDOWN);
    // ��������� ����������, ��������� callbcak ������� � �������� ��������� �� ��������
    palEnableLineEvent(GPIOC_BUTTON, PAL_EVENT_MODE_RISING_EDGE);
    palSetLineCallback(GPIOC_BUTTON, palcb_button, &arg);
    while (1) {
        chThdSleepMilliseconds(1);
    }
}
