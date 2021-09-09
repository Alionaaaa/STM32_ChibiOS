#include "ch.h"
#include "hal.h"
#include <chprintf.h>
// ��������� ������ ���������� � �������� � ������� format string
static const SerialConfig sd_st_cfg = {
  .speed = 115200,
  .cr1 = 0, .cr2 = 0, .cr3 = 0
};

static SerialDriver         *debug_serial = &SD3;
static BaseSequentialStream *debug_stream = NULL;

void debug_stream_init( void )
{
    sdStart( debug_serial, &sd_st_cfg );
    palSetPadMode( GPIOD, 8, PAL_MODE_ALTERNATE(7) );
    palSetPadMode( GPIOD, 9, PAL_MODE_ALTERNATE(7) );

    debug_stream = (BaseSequentialStream *)debug_serial;
}

void dbgprintf( const char* format, ... )
{
    if ( !debug_stream )
        return;

    va_list ap;
    va_start(ap, format);
    chvprintf(debug_stream, format, ap);
    va_end(ap);
}

// ������� ��������� �� ������� � ����������
SPIDriver* spi1 = &SPID1;

// ��������� ������������, circular ����� �������� � halconf.h, CS �������� ��� PAL_LINE
static SPIConfig conf = {
    // �� ���������� ����������
    .end_cb = NULL,
    // CS ������ �� ���� �4
    .ssline = PAL_LINE(GPIOA, 4),
    // �������� ������������ 16
    .cr1 = SPI_CR1_BR_1 | SPI_CR1_BR_0,
    // ����� ����� 8 ���
    .cr2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0
};

int main(void) {

    // �������� ����
    halInit();
    chSysInit();

    // �������� UART
    debug_stream_init();

    // ��������� �������
    spiStart(spi1, &conf);

    // ����������� ����. MISO, MOSI, SCLK � �������������� ����� �������� ��������, CS ������ �� ����� PUSHPULL
    palSetLineMode(PAL_LINE(GPIOA, 4), PAL_MODE_OUTPUT_PUSHPULL);
    palSetLineMode(PAL_LINE(GPIOA, 5), PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
    palSetLineMode(PAL_LINE(GPIOA, 6), PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);
    palSetLineMode(PAL_LINE(GPIOA, 7), PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);

    // �������������� ������
    uint8_t txbuf[5] = {0x60, 0x0F, 0x00, 0x01, 0x30};
    uint8_t rxbuf[6] = {0};
    uint8_t data_reg[1] = {0b11101000};

    // �������������� ������
    // �������� ����
    spiSelect(spi1);
    // ���������� ������ (������� ����� �������� � ����������������������, ����� ��������� 4 �������� ������������)
    spiSend(spi1, 5, txbuf);
    // ��������� ����
    spiUnselect(spi1);

    while (true) {
        // � �������� 10 ���� ������ ������
        chThdSleepMilliseconds(100);
        // �������� ����
        spiSelect(spi1);
        // ��������� ����� �������� ������ � ����������������������
        spiSend(spi1, 1, data_reg);
        // ������ 6 ����
        spiReceive(spi1, 6, rxbuf);
        // ��������� ����
        spiUnselect(spi1);
        // ������������ ���������� � ���������� �� ��������
        uint8_t i;
        // ������ ��� ������ � 8 �� 16 ���, ����� ������� ��������
        int16_t* temp = (int16_t*)rxbuf;
        float buf[3] = {0, 0, 0};
        // ��������� ������ � ������� � ������� � �������
        for(i = 0; i < 3; i++){
            buf[i] = (float)temp[i] * 0.07f;
        }
        // ������� ���������� ������ � ��������
        dbgprintf("x %.2f, y %.2f, z %.2f\r\n", buf[0], buf[1], buf[2]);
    }
}
