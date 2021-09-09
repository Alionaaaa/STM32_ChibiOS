#include "ch.h"
#include "hal.h"

// ������� ��������� �� ������� � ����������
static PWMDriver *pwm1Driver = &PWMD1;

PWMConfig pwm1conf = {
    // ������ ������� 500��� (������������ ����� 436, ��� ��� �������� ����������)
    .frequency = 500000,
    // ������ ����� 10000 ������ �������, �� ���� 0,02 �������
    .period    = 10000,
    // �� ����� ������������ ���������� �� ��������� �������
    .callback  = NULL,
    // �������� ������ ������ ���� � ��������� ACTIVE_HIGH � ��� �� �� ����� ������������ ����������
    .channels  = {
                  {.mode = PWM_OUTPUT_ACTIVE_HIGH, .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL},
                  {.mode = PWM_OUTPUT_DISABLED,    .callback = NULL}
                  },
    // �������� ������� � �����. �������� ��������, advanced ������� ������� ���������, ������� �������� bdtr ���
    .cr2        = 0,
    .dier       = 0
};

int main(void)
{
    chSysInit();
    halInit();
    // ��������� ���� E9 � �������������� ����� 1, ��� ��� ����� �� �������� ������ ������� ������� �������
    palSetLineMode( PAL_LINE( GPIOE, 9 ),  PAL_MODE_ALTERNATE(1) );
    // �������� ������ � ������
    pwmStart( pwm1Driver, &pwm1conf );
    // �������� ����� � ������������� ���������� 50% (����� ������ �������� ��� ���������� ������, 5000 / 10000 = 0,5)
    pwmEnableChannel( pwm1Driver, 0, 5000 );

    while (TRUE);
}
