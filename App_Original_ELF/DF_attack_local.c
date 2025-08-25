#include <msp430.h>
#define LENGTH 10
char total = 0;
char new_settings[11] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,'\r'};
void injectMedicinePort1(char* new_setting){
  volatile char settings[5] = {1,1,1,1,1}; 
  volatile char local[8] = {2,2,2,2,2,2,2,2};
  int i = 0;
  while(new_settings[i] != '\r'){
      settings[i] = new_setting[i];
      total = 'a' ^ settings[i];
      i++;
  }
  for (int j = 0; j < 8; j++){
      local[j] = total;
  }
}
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;
  P1DIR |= BIT0;              
  P1OUT &= 0xfe;
  P4DIR |= BIT7;
  injectMedicinePort1(new_settings);
  if (total != 0){
      P4OUT |= BIT7;
  }
  else{
      P1OUT |= BIT0;
  }
  return 0;
}
