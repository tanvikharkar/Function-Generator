// DAC Header

/**
* P1.7 Port 1.7 CS
* P1.5 UCB0CLK SCLK
* P1.6 UCB0SIMO COPI
*/

#define SPI_PORT P1
#define SPI_CS BIT7
#define SPI_SCLK BIT5
#define SPI_COPI BIT6
#define DAC_BITS 4096 // 12-bit DAC, 2^12 = 4096
#define MAX_VOLTS 3300 // 3300mV = 3.3V
#define FLAT_CALIBRATION 42 // Measured voltage ~42mV from desired voltage, 94?
#define THREE_PT_THREE 0xFFF
#define HIGH 3070
#define LOW 0

void DAC_init(void);
void DAC_write(uint16_t mVolts);
uint16_t DAC_volt_conv(uint16_t mVolts);
