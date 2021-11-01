#include "ch.h"
#include "hal.h"
// ���������� ����������
#include "chprintf.h"

// ���������� ������� �������� � ������ ����������� �����������
static SerialDriver *debug_serial = &SD3;
static BaseSequentialStream *debug_stream = NULL;

// ������������� ������ UART
static const SerialConfig sd_st_cfg = {
    .speed = 115200,
    .cr1 = 0,
    .cr2 = 0,
    .cr3 = 0
};

// ������� �������������
void debug_stream_init( void )
{
// ��������� �������
    sdStart( debug_serial, &sd_st_cfg );
// ��������� ���� � ��������� Rx, Tx
    palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
    palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );
// �������������� ��������� �� �����
    debug_stream = (BaseSequentialStream *)debug_serial;
}

// ������� �������� ������ � ��������
void dbgprintf( const char* format, ... )
{
// ���������, ��� debug_stream_init() ��������
    if ( !debug_stream )
    return;

// ���������� � chvprintf() ������ � ���� ����
    va_list ap;
    va_start(ap, format);
    chvprintf(debug_stream, format, ap);
    va_end(ap);
}

int main(void) {
    halInit();
    chSysInit();
// ��������� ������� � �������������� �����
    debug_stream_init();
    uint8_t i = 0;
    while (true) {
// ���������� � �������� ������������� ���������, � ����� �����������. �������� �������� �� ������� ����� ������ � �������� �������.
        dbgprintf("Test %s #%u or %0.1f\n\r", "message", i, (float)i + 0.5f);
        chThdSleepMilliseconds(1000);
        i++;
    }
}
