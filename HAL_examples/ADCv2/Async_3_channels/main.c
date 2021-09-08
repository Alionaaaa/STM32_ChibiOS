#include "ch.h"
#include "hal.h"

// ������� ������� ���������� �� ��������� ��������������
void endcbfun(ADCDriver* adcp)
{
    (void)adcp;
    // ������ ������� ���������. � ���� ��� �������� ����� ������ ��� �����, ��� ������ ������� ����� �� �����, ������ ����� ������
    palToggleLine(LINE_LED2);
}

// � ���� ��� ��������� ����� ������. � DMAFAILURE �������� � ��� ����� ������������ ������ ��������������
void errcbfun(ADCDriver* adcp, adcerror_t err)
{
    (void)adcp;
    if(err == ADC_ERR_DMAFAILURE)
        palToggleLine(LINE_LED1);
}

ADCConversionGroup adcconf1 = {
    // �������� ����������� �����
    .circular = TRUE,
    // ������ ��� ������
    .num_channels = 3,
    // ��� �� �� callback �������
    .end_cb = endcbfun,
    .error_cb = errcbfun,
    // Watchdog ��������, �� ������� ��� ������ �����
    .cr1 = ADC_CR1_AWDEN | ADC_CR1_AWDIE,
    // �� ���� �������������� �������� ��� ��� ChibiOS
    .cr2 = ADC_CR2_SWSTART,
    // ������ ������� ������ ���� ����� ��������������, ��, �����
    .smpr1 = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_144) | ADC_SMPR1_SMP_AN13(ADC_SAMPLE_15),
    .smpr2 = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_480),
    // ������� ��� ���������� ������
    .htr = 0xFFF,
    .ltr = 100,
    // ������ ������ ��� ������ ���� ���������
    .sqr1 = 0,
    .sqr2 = 0,
    .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN13)

};

// ������� �����. �������� ��������, � ��������� ������� �� ��� �� ��� �������� ��-�� �������, � ���� ��� ��-�� ���������� �������
static adcsample_t buf[3] = {0};

int main(void) {

    halInit();
    chSysInit();
    // ��������� ���� � ����� �� ����
    palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_INPUT_ANALOG);
    palSetLineMode(PAL_LINE(GPIOC, 0), PAL_MODE_INPUT_ANALOG);
    palSetLineMode(PAL_LINE(GPIOC, 3), PAL_MODE_INPUT_ANALOG);
    // �������� ������ ���
    adcStart(&ADCD1, NULL);
	// ������ ��������������
    adcStartConversion(&ADCD1, &adcconf1, buf, 1);
    // � ����� ��� ������� � ������. ����� ����� �������� ��������, ��� �������� ����������� � ����� ������ � �� ����� �� ���� ���� �� ������, ������� ���������� ���������� �� ������
    adcStartConversion(&ADCD1, &adcconf1, buf, 1);
    // ��� ��� �� ���������� ������ �������� ������������, ����� ����� ��������� ������
    adcStartConversion(&ADCD1, &adcconf1, buf, 1);
    while (true) {
	    // ��� � ������� ����� �������� ������ �� ������. ���� � �������� ������ ���������� �� ����� ������ ������ ���� 100 (��������� watchdog), ��� �����������, ������ � ������ ���������� �����������, � �� �� ���� ���� �� ������ (� ��� ���� �� ���������� ������ � ����������...)
        uint16_t ch1 = buf[0];
        uint16_t ch2 = buf[1];
        uint16_t ch3 = buf[2];
        chThdSleepMilliseconds(1000);

    }
}