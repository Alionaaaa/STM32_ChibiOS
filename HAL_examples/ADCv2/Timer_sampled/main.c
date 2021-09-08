#include "ch.h"
#include "hal.h"

// ��������� ���������� �������
#define NUM_OF_CHANNELS 3
// � ������� ���������
#define ADC_DEPTH 1502


void tim1cb(GPTDriver* gptp)
{
    (void)gptp;
    palToggleLine(PAL_LINE(GPIOB, 8));
}

// �������� ������ (�� ����� ��� �������� � halconf.h � mcu.h)
static const GPTConfig tim1_conf = {
    // ������� �������
    .frequency = 27000000,
    // ���������� �� �����������, �� �� ��� ��� ����� ������������
    .callback = tim1cb,
    // ������ ������� ������ TRGO �������
    .cr2 = TIM_CR2_MMS_1,
    .dier = 0
};

// �������� ���������� ��� ���
void endcbfun(ADCDriver* adcp)
{
    (void)adcp;
    // ���������� ��������� ������ �� ���� ���������, ������ ���-������ ����� ������ �� ��������� ������
    if(adcIsBufferComplete(&ADCD1))
        palToggleLine(PAL_LINE(GPIOB, 9));
    // ����� ���� ����� ��������� � ������� ����. DMA ����� ������ ��� � ������, ���������� ��� ���, � ���� ������ ������ ����� ��������������.
    // � ���� ������� ���� ������ ����������, ������ � ���� ������������� ������������� ��������
}

ADCConversionGroup adcconf1 = {
    // ����������� ����� ������
    .circular = TRUE,
    // ���������� ������� ����� ������, ����� �� ������ �� ���� ������ ��� �������������
    .num_channels = NUM_OF_CHANNELS,
    // ������ ������� ����������
    .end_cb = endcbfun,
    // ������ ����������, ���� �������� � �� �����
    .error_cb = NULL,
    // ��� ����������� ���
    .cr1 = 0,
    // ������ �������������� �� TRGO ������� �������, ��� ��� ������ ��� �� ��������
    .cr2 = ADC_CR2_EXTEN_RISING | ADC_CR2_EXTSEL_SRC(0b1001),
    // ����� ������� ������������� ��� ���� �������
    .smpr1 = ADC_SMPR1_SMP_AN10(ADC_SAMPLE_3) | ADC_SMPR1_SMP_AN13(ADC_SAMPLE_3),
    .smpr2 = ADC_SMPR2_SMP_AN3(ADC_SAMPLE_3),
    // ��� ������� watchdog ��� ��� ���� �����������
    .htr = 0,
    .ltr = 0,
    // ������� ����� ���������� ������ ��������� ������
    .sqr1 = 0,
    .sqr2 = 0,
    .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN3) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN10) | ADC_SQR3_SQ3_N(ADC_CHANNEL_IN13)

};

// ��������� �����. �������� �� ��, ��� ������ �� ���� ���� �������� ��������, ������ �� ��� ����������, � �������� ������ ������� �������������� ����������� �� ���������� �������
static adcsample_t buf[NUM_OF_CHANNELS * ADC_DEPTH] = {0};

// ������ ��� ���������� �� ������
uint16_t bufA[ADC_DEPTH] = {0};
uint16_t bufB[ADC_DEPTH] = {0};
uint16_t bufC[ADC_DEPTH] = {0};

int main(void) {

    halInit();
    chSysInit();
    // �������� ������ �������
    gptStart(&GPTD1, &tim1_conf);
    // � ���
    adcStart(&ADCD1, NULL);
    // �������� ���� �� ����
    palSetLineMode(PAL_LINE(GPIOA, 3), PAL_MODE_INPUT_ANALOG);
    palSetLineMode(PAL_LINE(GPIOC, 0), PAL_MODE_INPUT_ANALOG);
    palSetLineMode(PAL_LINE(GPIOC, 3), PAL_MODE_INPUT_ANALOG);
    // ��� ����� ���� ��� �������, � ������ ��� �� ���������
    palSetLineMode(PAL_LINE(GPIOB, 8), PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(PAL_LINE(GPIOB, 9), PAL_MODE_OUTPUT_PUSHPULL);
    // �������� ��������������
    adcStartConversion(&ADCD1, &adcconf1, buf, ADC_DEPTH);
    // ��������� ������ � �������� 27���. 27��� ������� ������������ � 1000 ������ �� ����������. � �����, ���� �������������� ����� ������ �������, ��� ������ �������, ������ ������� �� �������� (� ������ �����������, ���-�� ��� �� ���������� ���������). ���� �� ������������� �������������� ������� ��������� ����� ��������� �����������, ��� ��� ��� ������������� ����� ����� ������. ������ ��� � ����.
    gptStartContinuous(&GPTD1, 1000);

    while (true) {
        // ��� ������ �� �����, ��� ��� ��������� ������ ���������� �� ����� �����. ��� ������ ������������ ��� ����������� ������ � ������
        uint16_t i;
        for(i = 0; i < ADC_DEPTH; i++) {
            bufA[i] = buf[i * 3];
            bufB[i] = buf[i * 3 + 1];
            bufC[i] = buf[i * 3 + 2];
        }
        chThdSleepMilliseconds(1000);
    }
}
