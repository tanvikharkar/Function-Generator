// DCO Source Code

#include "msp.h"
#include "DCO.h"

void set_DCO(int freq)
{
  CS->KEY = CS_KEY_VAL; // Unlock CS Registers
  CS->CTL0 = 0;
  CS->CTL0 |= freq; // Reset Control 0 Register
  CS->CTL1 = 0;
  CS->CTL1 = (CS_CTL1_DIVM__1 | CS_CTL1_SELM__DCOCLK
           | CS_CTL1_SELS__DCOCLK | CS_CTL1_DIVS__1);
  CS->KEY = 0; // Lock CS Registers
}
