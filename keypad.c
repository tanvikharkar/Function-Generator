#include "msp.h"
#include "keypad.h"

void keypad_init(void){
  P5->SEL0 &= ~ROWS; // Set rows and columns to GPIO
  P5->SEL1 &= ~ROWS;
  P4->SEL0 &= ~COLUMNS;
  P4->SEL1 &= ~COLUMNS;
  
  P4->DIR &= ~COLUMNS; // Set P4.0 - 4.3 as inputs (columns)
  
  P5->DIR |= ROWS; // Set P5.4 - 5.7 as outputs (rows)
  
  P4->REN |= COLUMNS; // Enable pull down resistor on inputs (columns)
  P4->OUT &= ~COLUMNS;
  
  P5->REN |= ROWS; // Set all rows = 1
  P5->OUT |= ROWS;
  
  return;
}

char getkey(void){
  char columns, columns_read, rows; // Initialize variables
  
  P5->OUT |= ROWS;
  columns = P4->IN & COLUMNS; // Read columns to detect button press
  
  if (columns == 0x00){ // No button press detected
    return 0xFF;
  }
  
  rows = 0;
  P5->OUT &= ~ROWS; // Set all row outputs to 0
  
  while(rows < 4){
    P5->OUT = 0x10 << rows; // 0001_0000 << 1
    __delay_cycles(25); // Delay
    columns_read = P4->IN & COLUMNS; // Reading columns
    
    if (columns_read != 0){ // Button found
      break;
    }
    rows++; // Increment rows by 1 if button not found
  }
  
  if (rows == 0){
    if (columns == 0x01){
      return 0x01;
    }
    if (columns == 0x02){
      return 0x02;
    }
    if (columns == 0x04){
      return 0x03;
    }
    if (columns == 0x08){
      return 0x0A;
    }
  }
  
  if (rows == 1){
    if (columns == 0x01){
      return 0x04;
    }
    if (columns == 0x02){
      return 0x05;
    }
    if (columns == 0x04){
      return 0x06;
    }
    if (columns == 0x08){
      return 0x0B;
    }
  }
  
  if (rows == 2){
    if (columns == 0x01){
      return 0x07;
    }
    if (columns == 0x02){
      return 0x08;
    }
    if (columns == 0x04){
      return 0x09;
    }
    if (columns == 0x08){
      return 0x0C;
    }
  }
  
  if (rows == 3){
    if (columns == 0x01){
      return 0x0E;
    }
    if (columns == 0x02){
      return 0x00;
    }
    if (columns == 0x04){
      return 0x0F;
    }
    if (columns == 0x08){
      return 0x0D;
    }
  }
  
  if (rows > 4){
    return 0xFF;
  }
  
  return 0xFF;
}
