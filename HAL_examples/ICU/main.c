#include "ch.h"
#include "hal.h"
#include "stdlib.h"
// ��� ������ � ����������
#include "uart_debug.h"

// ������������ ��� ������ ��� ��������� �������
static PWMDriver* pwm1 = &PWMD1;

static PWMConfig pwm_conf = {
    .frequency = 50000,
    .period = 10000,
    .channels = {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL},
        {PWM_OUTPUT_DISABLED, NULL}
    },
    .cr2 = 0,
    .dier = 0
};

// ��������� ICU

// ����� ������ ���������� ������������ ��������
uint32_t duration = 0;

// ������������ ����� ������ ������
static ICUDriver* icu3 = &ICUD3;


// ���������� ������� ����������� �������
void cbWidth(ICUDriver *icup){
    // ������ ����� ������ �������� � �������������� ����������
    (void) icup;
    // ����� ����� ��������� �������� ���������� ��� ������������
    duration = icuGetWidthX(icu3);
    palSetLine(LINE_LED1);
}

void cbPeriod(ICUDriver *icup){
    (void) icup;
    palClearLine(LINE_LED1);
}

void cbOverflow(ICUDriver *icup){
    (void) icup;
    // ������ ������� ���������, ���� �������� �� �������
    palToggleLine(LINE_LED3);
}

//���������� ������������
static ICUConfig icu_conf = {
    // ������� 50 ���. �� ���� ���� ���� ������������� 20 ���
    .frequency = 50000,
    // �������� ������������ �������� �������� �������
    .mode = ICU_INPUT_ACTIVE_HIGH,
    // ���������� ����� ������� � ����� ������ ������ ���������
    .width_cb = cbWidth,
    .period_cb = cbPeriod,
    .overflow_cb = cbOverflow,
    // ��������� ����� ������� ��� ����� ������
    .channel = ICU_CHANNEL_1
    // �������� DIER � ARR ����������
};

int main(void) {
    // ��������� ChibiOS
    halInit();
    chSysInit();

    // �������� ���� � �������� ������ ������ ��� �����
    debug_stream_init();
    dbgprintf("Start\n\r");

    // �������� ������ ��� � ������ ��������� �� ���� E9
    palSetPadMode(GPIOE, 9, PAL_MODE_ALTERNATE(1));
    pwmStart(pwm1, &pwm_conf);
    pwmEnableChannel(pwm1, 0, PWM_PERCENTAGE_TO_WIDTH(pwm1, 2500));

    // �������� ������ ICU � ��������� ���� �6 � ����� ������� ������ �������� �������
    palSetPadMode(GPIOA, 6, PAL_MODE_ALTERNATE(2));
    icuStart(icu3, &icu_conf);

    // �������� ��������� � �������� ����� callback �������
    icuStartCapture(icu3);
    icuEnableNotifications(icu3);

    while (true) {
        // ������ �� ��������� ����� �� 1 �� 9
        char val = sdGetTimeout(&SD3, TIME_MS2I(500));
        // ��������� ������� ������ ������� ��� �� ��������� �� ���������
        // ��, ��� �������������, �� ������� ������ ������ �����
        sdGetTimeout(&SD3, TIME_IMMEDIATE);
        // ����������� ������ � �����. ���� ��������� �� �����, ����� 0
        uint8_t num = atoi(&val);
        // ��������� ����������� ���������� ������� ���� �� 0
        if(num != 0) pwmEnableChannel(pwm1, 0, PWM_PERCENTAGE_TO_WIDTH(pwm1, num*1000));
        // ������� ���������� �������� � ��������
        dbgprintf("%d\n\r", duration);
    }
}
