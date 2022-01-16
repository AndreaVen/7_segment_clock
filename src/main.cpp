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
short int leftButton=7;
short int centerButton=8;
short int rightButton=10;
bool debug=true;
int maxModes=1; // number of functionality of the clock
long currentMills;
bool firstEnter=1; // used in millis function to sync arduino millis and current second

/*
  overloading of a method print to easily debug the code    
*/

void print(String s){
  if (debug==true){
    Serial.println(s);
  }
}
void print(bool b){
  if (debug==true){
    Serial.println(b);
  }
}
void print(float f){
  if (debug==true){
    Serial.println(f);
  }
}
void print(long l){
  if (debug==true){
    Serial.println(l);
  }
}
void print(int i){
  if (debug==true){
    Serial.println(i);
  }
}
void print(short s){
  if (debug==true){
    Serial.println(s);
  }
}
void print(double d){
  if (debug==true){
    Serial.println(d);
  }
}



void setup() {
  rtc.halt(false);
  rtc.writeProtect(false);
  Serial.begin(9600);
  lc.shutdown(0,false);
  pinMode(leftButton,INPUT);
  pinMode(rightButton,INPUT);
  pinMode(centerButton,INPUT);
 // rtc.setTime(13,10,00); // set time for the first time
lc.setIntensity(0,12);
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
lc.setDigit(0,(int) startingDigit, (int) number/10,false); // first digit
bool dp;
if (startingDigit<4){
   dp=true;
}
else{
   dp =false;
}
lc.setDigit(0,(int) (startingDigit+1), (int) (number%10),dp); // second digit
}

// check the buttons and uptade the current state (functionality of the clock)
bool checkButtons()
{
  print((String)"checking buttons");
  bool left = digitalRead(leftButton);
  bool right = digitalRead(rightButton);
  bool center = digitalRead(centerButton);
  if (right == 0)
  {
    delay(100);
    if (state < maxModes)
    {
      state++;
    }
    else
    {
      state = 0;
    }
  }
  else if (left == 0)
  {
        delay(100);
    if (state > 1)
    {
      state--;
    }
    else
    {
      state = maxModes;
    }
  }
  else if (center == 0)
  {
        delay(100);
    // enter mode that changes the current time
  }
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

void displayCurrentTimeWithMillis(bool firstEnter){
  t=rtc.getTime();
  sec=t.sec;
  min=t.min;
  hour=t.hour;
  if (firstEnter){
    currentMills=0;
    firstEnter=0;
  }
  else{
    currentMills=millis();
  }
  displayNumber((unsigned short)0,hour); 
  displayNumber((unsigned short)2,min);
  displayNumber((unsigned short)4,sec);
  displayNumber((unsigned short)6,(unsigned short)currentMills/10);
  print((String)"current mills="+(unsigned short)currentMills/10) ;
  checkButtons();
}

void loop()
{
  unsigned short currentSecond = rtc.getTime().sec;
  while(1){

 print((String)"current state: "+state);
  if (state == 0)
    { // initial state, normal clock
      // display current time
      if (rtc.getTime().sec==currentSecond)
        {
          // check state pin, luminosity, and other settings
          checkButtons();
        }
      else
        {
          displayCurrentTime();
        currentSecond=rtc.getTime().sec;
        }
    }
    else if (state==1){
      displayCurrentTimeWithMillis(firstEnter);
      currentSecond=rtc.getTime().sec;
      firstEnter=0;
  }
  }


}


