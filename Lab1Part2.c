#include "addMap.h"
/* Your first program*/

#define LED_BASE	      0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define SW_BASE               0xFF200040
volatile int DELAY_LENGTH;

//hexDigits is an array containing values for displaying all 16 hexidecimal digits 
unsigned char hexDigits[16]  = {
    0x3F, 
    0x06, 
    0x5B, 
    0x5B, 
    0x66, 
    0x6D, 
    0x7D, 
    0x07, 
    0x7F, 
    0x67, 
    0x77, 
    0x7C, 
    0x39, 
    0x5E, 
    0x79, 
    0x71
};

//DisplayHex function takes an int value and uses it as index into the hexDigits array to get the display value 
void DisplayHex(int value)
{
    //this line points to the address of the first seven segment display 
    //HEX3_HEX0_BASE is the base address of the first seven segment display 
    volatile int * DISPLAY_ptr = (int *)HEX3_HEX0_BASE;
    *DISPLAY_ptr = hexDigits[value];
}

//ReadSwitches function reads the current state of the switches and returns the lower 4 bits of the value 
//the lower 4 bits are then used as the input for the DisplayHex function
int ReadSwitches(void)
{
    //SW_BASE is defined above to be 0xFF200040 which is the lower 4 switches  
    //the line below points to the base address of the first 4 switches
    volatile int * SWITCH_ptr = (int *)SW_BASE; 
    //the line below uses a bit mask to extract the lower 4 bits and discard the upper 4 bits 
    //0x0F = 00001111 
    unsigned int SWITCH_val = *(SWITCH_ptr) & 0x0F;
    return SWITCH_val;
}
	
int main(void) {
    volatile int * LED_ptr       = (int *)LED_BASE; // LED address
    volatile int delay_count; // volatile so the C compiler doesn't remove the loop
	volatile int status;
	
	DELAY_LENGTH = 700000;
	status = 0;
    *(LED_ptr) |= 0x1; //BITWISE OR, does and OR on every bit
	
    //infinite loop that checks state of system 
    while (1) 
	{
        //if delay count is 0 it will turn on the LED on or off and reset the delay count 
        if (delay_count == 0) {

            delay_count = DELAY_LENGTH;

            if (status == 0)
            {
                status = 1;
                *(LED_ptr) |= 0x1;
            }
            else
            {
                status = 0;
                *(LED_ptr) &= ~0x1;
                volatile int * HEX3_HEX0_ptr = (int *)HEX3_HEX0_BASE;
                *(HEX3_HEX0_ptr) = 0x0;
            }
        }
        //if statis is set to 1 the read switches function is called to get the current switches value
        else if(status == 1) {
            int switchVal = ReadSwitches();
            //displayHex function called with the current switches values as its argument 
			DisplayHex(switchVal);
        }

        //this delay loop just keeps the system from executing too quickly 
        for (delay_count = DELAY_LENGTH; delay_count != 0; --delay_count)
            ; 
    }
}