
//https://github.com/naroom/OpenSyringePump/blob/master/syringePump/syringePump.ino
//https://hackaday.io/project/1838-open-syringe-pump

// Controls a stepper motor via an LCD keypad shield.
// Accepts triggers and serial commands.
// To run, you will need the LCDKeypad library installed - see libraries dir.

// Serial commands:
// Set serial baud rate to 57600 and terminate commands with newlines.
// Send a number, e.g. "100", to set bolus size.
// Send a "+" to push that size bolus.
// Send a "-" to pull that size bolus.

#include <msp430.h>
//#include <RAhook.h>


/* -- Constants -- */
#define SYRINGE_VOLUME_ML 30.0
#define SYRINGE_BARREL_LENGTH_MM 80.0

#define THREADED_ROD_PITCH 1.25
#define STEPS_PER_REVOLUTION 200.0
#define MICROSTEPS_PER_STEP 16.0

#define SPEED_MICROSECONDS_DELAY 100 //longer delay = lower speed

#define	false	0
#define	true	1

#define	boolean	_Bool
#define three_dec_places( x ) ( (int)( (x*1e3)+0.5 - (((int)x)*1e3) ) )

long ustepsPerMM = MICROSTEPS_PER_STEP * STEPS_PER_REVOLUTION / THREADED_ROD_PITCH;
long ustepsPerML = (MICROSTEPS_PER_STEP * STEPS_PER_REVOLUTION * SYRINGE_BARREL_LENGTH_MM) / (SYRINGE_VOLUME_ML * THREADED_ROD_PITCH );


/* -- Pin definitions -- */
int motorDirPin = 2;
int motorStepPin = 3;

/* -- Enums and constants -- */
enum{PUSH,PULL}; //syringe movement direction

float mLBolus = 0.500; //default bolus size
float mLBigBolus = 1.000; //default large bolus size
float mLUsed = 0.0;
int mLBolusStepIdx = 3; //0.05 mL increments at first
//float mLBolusStep = mLBolusSteps[mLBolusStepIdx];
float mLBolusStep = 0.050;

//serial
char serialStr[80] = "";
boolean serialStrReady = false;
int serialStrLen = 0;

void setup(){
	WDTCTL = WDTPW | WDTHOLD;   
	__dint();

	P1DIR |= motorDirPin;
	P1DIR |= motorStepPin;
        P1DIR |= BIT0;

	P1OUT &= ~motorDirPin;
	P1OUT &= ~motorStepPin;
        P1OUT |= BIT0;
	/* Serial setup */
	P4SEL |= BIT4+BIT5;    //Configure UART in both TX and RX
	UCA1CTL1 |= UCSWRST;   // Put the USCI state machine in reset
	UCA1CTL1 |= UCSSEL_1;

	//Set the baudrate
	UCA1BR0 = 3;
	UCA1MCTL = 0xD6;
	UCA1CTL0 = 0x00;
	UCA1CTL1 &= ~UCSWRST;

	UCA1IE |= UCRXIE;
	__eint();
}
void processSerial();
void bolus(int direction);

void loop(int count){
        //check for LCD updates
//	readKey();

	//look for triggers on trigger lines
//	checkTriggers();

	//check serial port for new commands
//	readSerial();

	// EVAL1: get the time spent on processSerial() for 1 uL, with/without rewritting.
	// 	here we hard code mLBolus
	serialStr[0] = '+';
	serialStrReady = true;

	//unsigned long start, end;

	//start = usecs();

	if(serialStrReady){
		processSerial();
	}

	//end = usecs();
	//printf("mLBolus = %f, time usecs: %lu\n", mLBolus, end - start);
}

void processSerial(){
	//process serial commands as they are read in
	if(serialStr[0] == '+'){
		bolus(PUSH);
	}
	else if(serialStr[0] == '-'){
		bolus(PULL);
	}
	serialStrReady = false;
	serialStrLen = 0;
}

void delayMicroseconds(unsigned int usecs) {
    while (usecs > 0) {
        __asm("nop");
        usecs -= 1;
    }
}

void bolus(int direction){
	//Move stepper. Will not return until stepper is done moving.

	//change units to steps
	long steps = (mLBolus * ustepsPerML);
	if(direction == PUSH){
	        //led_on();
		//digitalWrite(motorDirPin, HIGH);
		P1OUT |= motorDirPin;
		P1OUT ^= BIT0;
	}
	else if(direction == PULL){
	        //led_off();
		//digitalWrite(motorDirPin, LOW);
		P1OUT &= motorDirPin;
		P1OUT &= 0xfe;
	}
        //float usDelay = SPEED_MICROSECONDS_DELAY; //can go down to 20 or 30
	unsigned int usDelay = SPEED_MICROSECONDS_DELAY; //can go down to 20 or 30

	for(long i=0; i < steps; i++){
		//digitalWrite(motorStepPin, HIGH);
		P1OUT |= motorStepPin;
		delayMicroseconds(usDelay);

		//digitalWrite(motorStepPin, LOW);
		P1OUT &= ~motorStepPin;
		delayMicroseconds(usDelay);
	}

}


int main(int argc, char **args) {
	//printf("Starting syringe pump");
	setup();

	int count = 1;
	//TA0CTL = TASSEL_1 + ID_0 + MC_2; // Start the timer with frequency of 32768 Hz
    	//TA0CTL = TASSEL_2 + ID_0 + MC_2; // Start the timer with Frequency of 1Mhz
	while(count < 62) {
		count += 10;
		loop(count);
	}
	//register unsigned int stop = TA0R; // Stop timer
	return 0;
}

