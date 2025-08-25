#include <msp430.h>
volatile char settings[8] = {0}; 
void injectMedicinePort1(char new_setting, int index){
  settings[index] = new_setting;
  int dose = 1;
  if (dose < 8){ 
     P1OUT ^= settings[index];
     dose++;
  }
  P4OUT |= BIT7;
  return ;
}
char add(char a, char b){
    return (a+b);
}
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR |= BIT0;              
  P1OUT &= 0xfe;
  P4DIR |= BIT7;
  volatile char input1 = 7;
  volatile char input2 = 3;
  volatile char outcome = add(input1, input2);
  volatile char length = 9;
  injectMedicinePort1(outcome,length);
  return 0;
}
