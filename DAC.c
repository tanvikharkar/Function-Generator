// DAC Source Code

#include "msp.h"
#include "DAC.h"

void DAC_init(void)
{
  EUSCI_B0->CTLW0 |= EUSCI_B_CTLW0_SWRST; // Put BUS into software reset
  EUSCI_B0->CTLW0 = (EUSCI_B_CTLW0_MSB // MSB first
                  | EUSCI_B_CTLW0_MST // Controller mode
                  | EUSCI_B_CTLW0_MODE0 // 3-pin SPI
                  | EUSCI_B_CTLW0_SYNC // Synchronous mode
                  | EUSCI_B_CTLW0_SSEL__SMCLK // Select SMCLK as source clock
                  | EUSCI_B_CTLW0_SWRST
                  | EUSCI_B_CTLW0_CKPL
                  ); // Keep software reset on
  
  EUSCI_B0->BRW = 0x01; // Divide clock by 1
  
  SPI_PORT->SEL0 |= (SPI_SCLK | SPI_COPI); // Configure SPI pins
  SPI_PORT->SEL1 &= ~(SPI_SCLK | SPI_COPI);
  
  SPI_PORT->SEL0 &= ~(SPI_CS); // Configure CS as GPIO
  SPI_PORT->SEL1 &= ~(SPI_CS);
  SPI_PORT->DIR |= (SPI_CS);
  SPI_PORT->OUT |= (SPI_CS); // Initialize high (active low)
  
  EUSCI_B0->CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);
}

void DAC_write(uint16_t mVolts)
{
  uint8_t loByte; // Split voltage value into upper and lower bytes
  uint8_t hiByte;
  uint16_t data;
  
  data = DAC_volt_conv(mVolts);
  
  loByte = data & 0xFF; // Clear upper byte of data
  hiByte = (data >> 8) & 0xFF; // Shift upper byte, clear upper byte, then configure bits
  hiByte |= 0x30;
  
  SPI_PORT->OUT &= ~(SPI_CS); // Set chip select low before transmission
  
  while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)); // Wait for TXBUF to be empty
  EUSCI_B0->TXBUF = hiByte; // Put upper byte into buffer
  
  while(!(EUSCI_B0->IFG & EUSCI_B_IFG_TXIFG)); // Wait for TXBUF to be set (empty)
  EUSCI_B0->TXBUF = loByte; // Put lower byte into buffer
  
  while(!(EUSCI_B0->IFG & EUSCI_B_IFG_RXIFG)); // Wait for RXBUF to be set (empty)
  SPI_PORT->OUT |= SPI_CS; // Set chip select high after transmission
  
  //__delay_cycles(50);
}

uint16_t DAC_volt_conv(uint16_t mVolts)
{
  uint16_t data = ((mVolts + FLAT_CALIBRATION) * DAC_BITS / MAX_VOLTS); // Data conversion from keypad reading voltage output
  return data;
}
