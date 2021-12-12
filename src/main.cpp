#include <Arduino.h>
#include <DS1302.h>
#include <LedControl.h>




DS1302 rtc(5, 3, 13); // RST, DAT, CLOCK
LedControl lc=LedControl(1,2,3,4);

unsigned short state=0;
Time t;
unsigned short sec;
unsigned short min;
unsigned short hour;



void setup() {
  // put your setup code here, to run once:
  rtc.halt(false);
  rtc.writeProtect(false);
}

void loop() {
  
if (state==0){ // initial state, normal clock
// display current time

unsigned short currentSecond=rtc.getTime().sec;
if (rtc.getTime().sec==currentSecond){

  // check state pin, luminosity, and other settings
}
else{
  // display new time
}

}

}


/*
display a 2 gidit number 
Params: 
* first digit of to display 
*the number that will be split 
example:
displayNumber(0,12)
displays the number 1 on the digit 0 and the number 2 on the following digit (1) 
*/
void displayNumber(unsigned short startingDigit,unsigned short number){
// startingDigit=startingDigit;

lc.setDigit(0,(int) startingDigit, (int) number%10,false);


}

void displayCurrentTime(){
t=rtc.getTime();
sec=t.sec;
min=t.min;
hour=t.hour;
displayNumber((unsigned short)0,sec);

}


