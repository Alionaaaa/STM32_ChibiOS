#include "ch.h"
#include "hal.h"

// ������ callback �������
void cb_fun1(PWMDriver *pwmp)
{
    (void)pwmp;
    palToggleLine(LINE_LED1);
}

// ������ callback �������
void cb_fun2(PWMDriver *pwmp)
{
    (void)pwmp;
    palToggleLine(LINE_LED2);
}

// ��������� ������������
static PWMConfig pwmcfg = {
  // ������� 10���
  .frequency = 100000,
  // ������ 5000 ������, ��� 0,05�
  .period = 5000,
  // ������ �� �������
  .callback = cb_fun1,
  .channels = {
   // �������� ������ ����� � ��������������� ������, � ������� ������ �������
   {PWM_OUTPUT_ACTIVE_HIGH | PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH, cb_fun2},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  .cr2 = 0,
  // ������ ������ ����� 2���
  .bdtr = 0b11010110,
  .dier = 0
};
// ��������� ��������� �� ������ ������� � ����������
static PWMDriver *pwm = &PWMD1;

int main(void) {
    halInit();
    chSysInit();
    // ��������� ���
    pwmStart(pwm, &pwmcfg);
    // ��������� ����� ������ �������
    pwmEnablePeriodicNotification(pwm);
    // ������������� ���� ������� ������ ������� ������� � ����� ���
    palSetLineMode(PAL_LINE(GPIOE, 9), PAL_MODE_ALTERNATE(1));
    palSetLineMode(PAL_LINE(GPIOE, 8), PAL_MODE_ALTERNATE(1));
    // �������� ��������� ������� �� �����
    pwmEnableChannel(pwm, 0, PWM_PERCENTAGE_TO_WIDTH(pwm, 5000));
    // ��������� ����� ������ �������
    pwmEnableChannelNotification(pwm, 0);
    uint16_t duty = 0;
    while (true) {
        // ������ ������������ ������
        duty += 100;
        if(duty > 10000) duty = 0;
        pwmEnableChannel(pwm, 0, PWM_PERCENTAGE_TO_WIDTH(pwm, duty));
        chThdSleepMilliseconds(200);
    }
}
