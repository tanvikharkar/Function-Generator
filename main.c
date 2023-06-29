#include "msp.h"
#include "DCO.h"
#include "DAC.h"
#include "keypad.h"

#define NONE 0xFF // No key pressed
#define SAMPLES 1300 // Number of samples in arrays, 1300
#define TIMER_A0_UP 350
#define USER_DELAY 10000000

int duty_cycle = 5; // Duty cycle = 50%
int frequency = 1; // Frequency = 100Hz
int i = 0; // Variable for indexing through arrays
uint8_t wave = 0; // Wave = square wave
//int duty_cycle_samples = SAMPLES * duty_cycle;

int gen_array[]; // General array
int square_array_high[]; // Initialize arrays
int square_array_low[];
int tri_array[SAMPLES] = {
// precalculated values
};

int saw_array[SAMPLES] = {
// precalculated values
};

int sine_array[SAMPLES] = {
// precalculated values
};

void TA0_0_IRQHandler(void);

void main(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
  
  set_DCO(FREQ_24_MHz); // Set f = 24MHz
  keypad_init(); // Initialize keypad
  DAC_init(); // Initialize DAC
  
  char key = NONE; // Start w/ assumption that no key is pressed
  
  TIMER_A0->CTL |= (TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC__CONTINUOUS); // Select SMCLK for Timer A, cont. mode
  TIMER_A0->CCTL[0] |= TIMER_A_CCTLN_CCIE; // Enable interrupts
  TIMER_A0->CCR[0] += TIMER_A0_UP;
  NVIC->ISER[0] = 1 << (TA0_0_IRQn); // Enable flag for Timer A
  __enable_irq(); // Enable global interrupts
  
  typedef enum // Define FSM States
  {
    SQUARE,
    SINE,
    TRIANGLE,
    SAWTOOTH
  } STATE_TYPE;
  
  STATE_TYPE state = SQUARE; // Show SQUARE wave upon power up
  
  while(1)
  {
    switch(state) // Switch statement to cycle through FSM cases
    {
      case SQUARE:
        wave = 1; // For ISR to know when to write HIGH / LOW for square wave
        key = getkey(); // Detect keypad presses
        if (key >= 1 && key <= 5) // Keys 1-5: frequency = 100 * key value
        {
          frequency = key;
          state = SQUARE;
          break;
        }
        
        else if (key == CHG_DUTY_CYCLE) // If any of the duty cycle keys pressed
        {
          if (key == RST_DUTY_CYCLE) // If 50% duty cycle key pressed
          {
            duty_cycle = 5; // Reset duty cycle to 50%
            state = SQUARE; // Stay in SQUARE state
            __delay_cycles(USER_DELAY); // Delay to debounce button presses
            break;
          }
          
          else if (key == INC_DUTY_CYCLE) // If increase duty cycle key pressed
          {
            if (duty_cycle < 9) // Do not increment duty cycle greater than 90%
            duty_cycle++; // Increment duty cycle
            state = SQUARE; // Stay in SQUARE state
            __delay_cycles(USER_DELAY); // Button debouncer
            break;
          }
          
          else if (key == DEC_DUTY_CYCLE) // If decrease duty cycle key pressed
          {
            if (duty_cycle > 1) // Do no decrement duty cycle less than 10%
            {
              duty_cycle--; // Decrement duty cycle
            }
            state = SQUARE; // Stay in SQUARE state
            __delay_cycles(USER_DELAY); // Button debouncer
            break;
          }
          
          else if (key == SINE_KEY) // If sine key pressed
          {
            state = SINE; // state = SINE
            break;
          }
          
          else if (key == TRI_KEY) // If triangle key pressed
          {
            state = TRIANGLE; // state = TRIANGLE
            break;
          }
          
          else if (key == SAW_KEY) // If sawtooth key pressed
          {
            state = SAWTOOTH; // state = SAWTOOTH
            break;
          }
          
          else
          {
            state = SQUARE; // default for SQUARE state: stay in SQUARE state
          }
        }
        break;
        
      case SINE:
        wave = 0; // For ISR to know not to create a square wave
        key = getkey(); // Detect keypad presses
        for (i = 0; i < SAMPLES; i++)
        {
          gen_array[i] = sine_array[i]; // Populate general array for DAC_write()
        }
        
        if (key >= 1 && key <= 5)
        {
          frequency = key; // Set frequency equal to key press
          state = SINE;
          break;
        }
        
        else if (key == SQUARE_KEY) // If square key pressed
        {
          state = SQUARE; // state = SQUARE
          break;
        }
        
        else if (key == TRI_KEY) // If triangle key pressed
        {
          state = TRIANGLE; // state = TRIANGLE
          break;
        }
        
        else if (key == SAW_KEY) // If sawtooth key pressed
        {
          state = SAWTOOTH; // state = SAWTOOTH
          break;
        }
        
        else
        {
          state = SINE; // default for SINE state: stay in SINE state
        }
        break;
        
      case TRIANGLE:
        wave = 0; // For ISR to know not to create a square wave
        key = getkey(); // Detect keypad presses
        for (i = 0; i < SAMPLES; i++)
        {
          gen_array[i] = tri_array[i]; // Populate general array for DAC_write()
        }
        if (key >= 1 && key <= 5)
        {
          frequency = key; // Set frequency equal to key press
          state = TRIANGLE;
          break;
        }
        else if (key == SQUARE_KEY) // If square key pressed
        {
          state = SQUARE; // state = SQUARE
          break;
        }
        else if (key == SINE_KEY) // If sine key pressed
        {
          state = SINE; // state = SINE
          break;
        }
        else if (key == SAW_KEY) // If sawtooth key pressed
        {
          state = SAWTOOTH; // state = SAWTOOTH
          break;
        }
        else
        {
          state = TRIANGLE; // default for TRIANGLE state: stay in TRIANGLE state
        }
        break;
        
      case SAWTOOTH:
        wave = 0; // For ISR to know not to create a square wave
        key = getkey(); // Detect keypad presses
        for (i = 0; i < SAMPLES; i++)
        {
          gen_array[i] = saw_array[i]; // Populate general array for DAC_write()
        }
        if (key >= 1 && key <= 5)
        {
          frequency = key; // Set frequency equal to key press
          state = SAWTOOTH;
          break;
        }
        else if (key == SQUARE_KEY) // If square key pressed
        {
          state = SQUARE; // state = SQUARE
          break;
        }
        else if (key == TRI_KEY) // If triangle key pressed
        {
          state = TRIANGLE; // state = TRIANGLE
          break;
        }
        else if (key == SINE_KEY) // If sine key pressed
        {
          state = SINE; // state = SINE
          break;
        }
        else
        {
          state = SAWTOOTH; // default for SAWTOOTH state: stay in SAWTOOTH state
        }
        break;
    }
  }
}

void TA0_0_IRQHandler(void)
{
  if (TIMER_A0->CCTL[0] & TIMER_A_CCTLN_CCIFG) // Check if CCIFG(CCTL[0]) is high (interrupted)
  {
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG; // Clear interrupt flag
    
    if (wave == 1) // For square wave
    {
      if (i < (SAMPLES * duty_cycle / 10)) // Write HIGH according to current duty cycle
      {
        DAC_write(HIGH);
        i += frequency; // Increment index by frequency
        TIMER_A0->CCR[0] += TIMER_A0_UP; // Increment timer
      }
      
      else
      {
        DAC_write(LOW); // Write LOW according to current duty cycle
        i += frequency; // Increment index by frequency
        TIMER_A0->CCR[0] += TIMER_A0_UP; // Increment timer
        if (i > SAMPLES) // Check if index > SAMPLES
          i = 0; // Reinitialize i = 0
      }
    }
    
    else // For sine, triangle, and sawtooth waveforms
    {
      if (i < SAMPLES) // Check if index < samples
      {
        DAC_write(gen_array[i]); // Write values from populated general array
        i += frequency; // Increment index by frequency
        TIMER_A0->CCR[0] += TIMER_A0_UP; // Increment timer
      }
      
      else // index > SAMPLES
      {
        i = 0; // Reinitialize index to zero
        TIMER_A0->CCR[0] += TIMER_A0_UP; // Increment timer
      }
    }
  }
}
