// Keypad Header

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define COLUMNS (BIT0 | BIT1 | BIT2 | BIT3) // Set column bits
#define ROWS (BIT4 | BIT5 | BIT6 | BIT7) // Set row bits
#define DEC_DUTY_CYCLE 0x0E // Key * = decreases duty cycle by 10%
#define RST_DUTY_CYCLE 0x00 // Key 0 = resets duty cycle to 50%
#define INC_DUTY_CYCLE 0x0F // Key # = increases duty cycle by 10%
#define CHG_DUTY_CYCLE 0x0E | 0x00 | 0x0F // Use to check if any duty cycle keys are pressed
#define SINE_KEY 0x06 // Key 6 = SINE wave
#define TRI_KEY 0x07 // Key 7 = TRIANGLE wave
#define SAW_KEY 0x08 // Key 8 = SAWTOOTH wave
#define SQUARE_KEY 0x09 // Key 9 = SQUARE wave

// Define Functions
void keypad_init(void);
char getkey(void);

#endif /* KEYPAD_H_ */
