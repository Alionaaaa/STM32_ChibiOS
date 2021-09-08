#include "ch.h"
#include "hal.h"

// ������� ������� ���������� �� ��������� ��������������
void endcbfun(ADCDriver* adcp)
{
    (void)adcp;
    // ������ ������� ���������
    palToggleLine(LINE_LED2);
}

// � ��������� ������� ���� ���������� ������
void errcbfun(ADCDriver* adcp, adcerror_t err)
{
    (void)adcp;
    // ���� ������ ������ watchdog, ����� ������ ������ ���������
    if(err == ADC_ERR_WATCHDOG)
        palToggleLine(LINE_LED1);
}

ADCConversionGroup adcconf1 = {
    // ��� ������������ ������
    .circular = FALSE,
    // ��� ��� 1 �����
    .num_channels = 1,
    // ������ ������������ ����� �������
    .end_cb = endcbfun,
    .error_cb = errcbfun,
    // ������� watchdog, �������� ��� ��������� ���� ����������, ������ ��� �������� �� ������ ����� �� ����������� � ������ ������ �����
    .cr1 = ADC_CR1_AWDEN | ADC_CR1_AWDIE | ADC_CR1_AWDSGL | ADC_CR1_AWDCH_0 | ADC_CR1_AWDCH_1,
    // ������� ������ �������������� �� ChibiOS
    .cr2 = ADC_CR2_SWSTART,
    // �������� ����� ������ ����� ��������������, 480 ������ (������ ��� �����)
    .smpr1 = 0,
    .smpr2 = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_480),
    // ������� ������� ��� ���������� �������. ������ ������� ���, ����� �������� �������� �� ������������� ����������
    .htr = 0xFFF,
    .ltr = 1000,
    // ������ ������ �����
    .sqr1 = 0,
    .sqr2 = 0,
    .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3)

};

// ��������� ����� ��� ���������� ��������������
static adcsample_t buf[4] = {0, 0, 0, 0};

int main(void) {
    halInit();
    chSysInit();
    // ��������� ���� � ����� �� ����� � ������� ���
    palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, NULL);
    while (true) {
        // ��� � ������� ������ ��������������. ���� ���������� ���� ������� ������, �� ������� ������ �������� (LINE_LED2, �� ���� ��� ��), ���� ����, �� ������� ������ �������� (LINE_LED1, �� ���� ��� �� ��). ������ ������ �������������� � ����� ������ ���������� � ������ ������� ���������, �� ��� �������� ����������, �������������� ����������� � ���������� �������� ������ �� ���������. � ������ �������, ���� ������ ������� ��� �� ������, ������������ ������ ������ ����� � ��� �� ����� ������.
        adcConvert(&ADCD1, &adcconf1, buf, 4);
        chThdSleepMilliseconds(1000);
    }
}