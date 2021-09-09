#include "ch.h"
#include "hal.h"

// ����� ������������ ������ � ������ �������
GPTDriver *timer1 = &GPTD1;
GPTDriver *timer3 = &GPTD3;

// ���������� ��� ������� ����������� ����������

// ������ ������� ������ ������� ���������
void cbgptfun1(GPTDriver *gptp)
{
    (void)gptp;
    palToggleLine(LINE_LED1);
}

// ������ ������� ������ ��������� � ������ ��� ������������� ������
void cbgptfun2(GPTDriver *gptp)
{
    (void)gptp;
    // ������ ������ ������� �������
    uint16_t T = gptGetIntervalX(timer1);
    gptStartOneShotI(timer3, T);
    palToggleLine(LINE_LED2);
}

// ����������� ������� ������� ������� 50_000�� (������������ 4320, ����� �����, ������ ��� 2^16) � ��������� ������ ������� ��� ���������� ����������
GPTConfig gpt1_conf = {
    .frequency = 50000,
    .callback = cbgptfun1,
    .cr2 = 0,
    .dier = 0
};

// ��� �������� ������� ��������� ����� �� (��� ��� �� ������� ����� ������ ������������, ��� ��� ������ ����, �� ������ API ����� �� �����), �� ������ ������ �������
GPTConfig gpt3_conf = {
    .frequency = 50000,
    .callback = cbgptfun2,
    .cr2 = 0,
    .dier = 0
};

int main(void) {

    halInit();
    chSysInit();
    // ������� ��� �������
    gptStart(timer1, &gpt1_conf);
    gptStart(timer3, &gpt3_conf);
    // ������ �������� � ����������� ������ � �������� 2��
    gptStartContinuous(timer1, 25000);
    // ��������� ������ ������� ������� (������ ��� �����)
    uint16_t T = gptGetIntervalX(timer1);
    // �������� ������ ������ � ��� �� ��������
    gptStartOneShot(timer3, T);
    while (true) {
        // ���������, ��� ��� �������� ��������� �������
        chThdSleepMilliseconds(1000);
    }
}
