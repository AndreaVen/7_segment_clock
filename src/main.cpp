#include <Arduino.h>
#include <DS1302.h>
#include <LedControl.h>




DS1302 rtc(5, 3, 13); // RST, DAT, CLOCK
LedControl lc=LedControl(12,11,10,1); // din(miso),clock(mosi),csLoad

unsigned short state=0;
Time t;
unsigned short sec;
unsigned short min;
unsigned short hour;



void setup() {
  // put your setup code here, to run once:
  rtc.halt(false);
  rtc.writeProtect(false);
  Serial.begin(9600);
  lc.shutdown(0,false);
 // rtc.setTime(13,10,00);

/* Set the brightness to a medium values */

lc.setIntensity(0,12);

/* and clear the display */

lc.clearDisplay(0);
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
Serial.println("entrato in display number");
//lc.setDigit(0,1,2,false);// prima cifra boh ,faccio print di due sul digit 1 
lc.setDigit(0,(int) startingDigit, (int) number/10,false);
bool dp;
if (startingDigit<4){
   dp=true;
}
else{
   dp =false;
}
lc.setDigit(0,(int) (startingDigit+1), (int) (number%10),dp);
//lc.setDigit(0,(int) startingDigit,2,false);

}

void displayCurrentTime(){
t=rtc.getTime();
sec=t.sec;
min=t.min;
hour=t.hour;



displayNumber((unsigned short)0,hour); 
displayNumber((unsigned short)2,min);
displayNumber((unsigned short)4,sec);


}

void loop()
{
    unsigned short currentSecond = rtc.getTime().sec;

  while(1){
  if (state == 0)
  { // initial state, normal clock
    // display current time

    if (rtc.getTime().sec==currentSecond)
    {
      // check state pin, luminosity, and other settings
      
    }
    else
    {
      Serial.println("entrato nel ciclo di condizionem");
      currentSecond= rtc.getTime().sec;
      Serial.println("secondo cambiato:");
            Serial.println(rtc.getTime().sec);

      currentSecond=rtc.getTime().sec;
      displayCurrentTime();
     

    }




  
  
  }

}
}


