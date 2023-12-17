#include <Arduino.h>
#include <DHT.h>
#include "uRTCLib.h"
#include "SevenSegmentDisplay.h"
void chek_and_set_time_down(short int );
void chek_and_set_time_up(short int );
void check_and_set_light_up();
void check_and_set_light_down();
uRTCLib rtc;





#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT22
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
unsigned short state=0;
unsigned short sec;
unsigned short min;
unsigned short hour;
short int leftButton=7;
short int centerButton=8;
short int rightButton=9;
short int lux_pin=2;
bool debug=true;
int maxModes=4; // number of functionality of the clock
long currentMills;
unsigned long startingMillis=0;
short int central_button_counter=0; // once the hour and minutes has been set you can resume normal mode
float oldTemp=0;
int oldHum=0;
bool firstEnter=1; // used in millis function to sync arduino millis and current second
int light_status=12; // 0=auto
int number_of_light_mode=15;

SevenSegmentDisplay display;


/*
  overloading of a method print to easily debug the code    
*/

void print(String s){
  if (debug==true){
    Serial.println(s);
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
  
  Wire.begin();
  Serial.begin(9600);
  dht.begin();  
  pinMode(leftButton,INPUT);
  pinMode(rightButton,INPUT);
  pinMode(centerButton,INPUT);
  pinMode(A2,INPUT);
  URTCLIB_WIRE.begin();
  // for example to set January 13 2022 at 12:56 you would call:
  rtc.set_rtc_address(0x68);
  display.init();


}






// check the buttons and uptade the current state (functionality of the clock)

bool check_right(){
  int right = digitalRead(rightButton);
  if (right==0){
    return true;
  }
  else{
    return false;
  }
}
bool check_left(){
  int left =digitalRead(leftButton);
  if (left==0){
    return true;
  }
  else{
    return false;
  }
}
void check_center(){
bool center = digitalRead(centerButton);
  if (center==0){
    central_button_counter++;
    print("aumentato center counter =");
    Serial.println(central_button_counter);
    delay(300);
  }
}



bool checkButtons()
{
  bool left = digitalRead(leftButton);
  bool right = digitalRead(rightButton);
  bool center = digitalRead(centerButton);
  if (right == 0)
  {
    print( (String)"Right");
    print((String)"old state= "+state);
  display.clear();
    delay(300);
    if (state < maxModes)
    {
      state++;
    }
    else
    {
      state = 0;
    }
        print((String)"new state= "+state);
    return true;
  }
  else if (left == 0)
  {
    print((String)"left");
            print((String)"old state= "+state);

    display.clear();

        delay(300);
    if (state > 0)
    {
      state--;
    }
    else
    {
      state = maxModes;
    }
            print((String)"new state= "+state);
    return true;
  }
  else if (center == 0)
  {
      print((String)"center");
      display.clear();
      state=42;
      delay(300);
      return true;
    // enter mode that changes the current time
  }
  else{
    return false;
  }
}
void blink_time_on(short int startingDigit){
  long unsigned int now=millis();
  unsigned int time_on=1000;
  while(millis()-now<time_on){
    rtc.refresh();
    sec=rtc.second();
    min=rtc.minute();
    hour=rtc.hour();
    display.displayNumber((unsigned short)0,hour,false); 
    display.displayNumber((unsigned short)2,min,true);
    chek_and_set_time_up(startingDigit);
    chek_and_set_time_down(startingDigit);
  }
}

void blink_light_on(short int startingDigit){
  long unsigned int now=millis();
  unsigned int time_on=1000;
  while(millis()-now<time_on){
display.displayLightStatus();
    check_and_set_light_up();
    check_and_set_light_down();
  }
}

int format_hour(int increment){
  if (increment>0){
    //quanto è chiamato format_hour?
    if (rtc.hour()>22){ // it means now is 23:XX I have to set 00:XX
      return 0;
    }
    else{
      return rtc.hour()+1;
    }
  }
  else{
        if (rtc.hour()==0){ // it means now is 00:XX I have to set 23:XX
      return 23;
    }
    else{
      return rtc.hour()-1;
    }
  }
}
int format_minutes(int increment){
if (increment>0){
    if (rtc.minute()==59){ // it means now is XX:59I have to set XX:00
      return 0;
    }
    else{
      return rtc.minute()+1;
    }
  }
  else{
        if (rtc.minute()==0){ // it means now is XX:00 I have to set XX:59
      return 59;
    }
    else{
      return rtc.minute()-1;
    }
  }


}


void chek_and_set_time_up(short int startingDigit){
  rtc.refresh();
  if (check_right()){
    min=rtc.minute();
    hour=rtc.hour();
    sec=rtc.second();
    if (startingDigit==0){ // setting hour
      int new_hour= format_hour(1);
      rtc.set(sec, min, new_hour, 5, 13, 1, 22);
    }
    else if (startingDigit==1){ // setting minutes
      int new_minute=format_minutes(1);
      rtc.set(sec, new_minute, hour, 5, 13, 1, 22);
    }
    delay(300);
  }
  check_center();

}



void set_light_status(){
  if (light_status>0)
  {
    lc.setIntensity(0,light_status);
  }
  else if (light_status==0){
    Serial.println("livello 0");
    set_light_auto();
  }


}


void check_and_set_light_up(){
if (check_right()){
   if (light_status<number_of_light_mode){
    light_status++;
set_light_status();
display.displayLightStatus();
   }
   else{
    light_status=0;
set_light_status();
display.displayLightStatus();

   } 
     delay(300);
  }
  check_center();
}


void check_and_set_light_down(){
if (check_left()){
   if (light_status>0){
    light_status--;
set_light_status();
display.displayLightStatus();
   }
   else{
    light_status=number_of_light_mode;
set_light_status();
display.displayLightStatus();
   } 
    delay(300);
  }
  check_center();
}

void chek_and_set_time_down(short int startingDigit){
  rtc.refresh();
  if (check_left()){
    min=rtc.minute();
    hour=rtc.hour();
    sec=rtc.second();
    if (startingDigit==0){ // setting hour
      int new_hour= format_hour(-1);
      rtc.set(sec,min,new_hour, 5, 13, 1, 22); // set time hh:mm:ss
    }
    else if (startingDigit==1){ // setting minutes
      int new_minute=format_minutes(-1);
      rtc.set(sec,new_minute,hour, 5, 13, 1, 22);
    }

    delay(300);
    
  }
      check_center();

}


void  display_noting(short int startingDigit){
// display only the time NOT on the starting digit i.e if startingDigit=0 the function displays the minutes but not the hours
display.clear();
long unsigned int now=millis();
unsigned int time_on=150;
while(millis()-now<time_on){
  sec=rtc.second();
  min=rtc.minute();
  hour=rtc.hour();
  if (startingDigit==0){
    display.displayNumber((unsigned short)2,min,true);
  }
  else if (startingDigit==1){
    display.displayNumber((unsigned short)0,hour,true); 
  }
  chek_and_set_time_up(startingDigit);
  chek_and_set_time_down(startingDigit);

}
}



void blink_off(short int startingDigit){
  display_noting(startingDigit);

}

void blink_light_off() {
display.clear();
long unsigned int now=millis();
unsigned int time_on=150;
while(millis()-now<time_on){
  display.setChar(0,0,'L',true);
  check_center();
  check_and_set_light_down();
  check_and_set_light_up();
  } 
}





void set_time()
{ 
  print("iniziato set_time");
  display.clear();
  while(central_button_counter<2){
    print("ancora in set time causa valore counter=");
    Serial.println(central_button_counter);
    blink_time_on(central_button_counter);
    blink_off(central_button_counter);
  }
  print("uscendo da set time");
  state=1;
  central_button_counter=0; // once the hour and minutes has been set you can resume normal mode

}


void set_light(){
  print("iniziato set_light");
  display.clear();
  while(central_button_counter<1){
    lc.setChar(0,0,'L',true);
    print("ancora in set light causa valore counter=");
    Serial.println(central_button_counter);
    blink_light_on(central_button_counter);
    blink_light_off();
  }
  print("uscendo da set light");
  state=1;
  central_button_counter=0; // once the hour and minutes has been set you can resume normal mode

}

void displayCurrentTime(){
  sec=rtc.second();
  min=rtc.minute();
  hour=rtc.hour();
  display.displayNumber((unsigned short)0,hour,true); 
  display.displayNumber((unsigned short)2,min,true);
  display.displayNumber((unsigned short)4,sec,false);
}

void displayCurrentTimeWithMillis(bool firstEnter){
  sec=rtc.second();
  min=rtc.minute();
  hour=rtc.hour();

  if (firstEnter){
    startingMillis=millis();
    display.displayNumber((unsigned short)6,(unsigned short)0,false);
    currentMills=millis();
  }
  else{
    currentMills=millis()-startingMillis; 
    display.displayNumber((unsigned short)6,(unsigned short)currentMills/10,false);
  }
  display.displayNumber((unsigned short)0,hour,true); 
  display.displayNumber((unsigned short)2,min,true);
  display.displayNumber((unsigned short)4,sec,false);
  checkButtons();
  
}
 void displayTemperatureAndTime(){
  min=rtc.minute();
  hour=rtc.hour();
  display.displayNumber((unsigned short)0,hour,true); 
  display.displayNumber((unsigned short)2,min,false);

  if (oldTemp< 1){ // if this is the first time entering the method just print the instant temperature
    float temp= dht.readTemperature();
    int integer= floor(temp); 
    int decimal= int((temp-integer)*100);
    Serial.print("old: ");
    Serial.println(oldTemp);
    display.displayNumber((unsigned short)5,integer,true);
    display.displayNumber((unsigned short)7,decimal,false);
    oldTemp=temp;
  }
  else
  {
    int integer= floor(oldTemp); 
    int decimal= int((oldTemp-integer)*100);
    display.displayNumber((unsigned short)5,integer,true);
    display.displayNumber((unsigned short)7,decimal,false);
    long startTime=millis();
    float meanTemperature=dht.readTemperature();
    int n=1;
    while(millis()-startTime<20000 && !checkButtons()){ // update every X seconds or when a button is pressed
      n++;
      meanTemperature = meanTemperature*(n-1)/n + meanTemperature/n; // iterative mean
      delay(10);

    }
    Serial.println(meanTemperature);
    float temp= meanTemperature;
    oldTemp=temp;
    integer= floor(temp); 
    decimal= int((temp-integer)*100);
    display.displayNumber((unsigned short)5,integer,true);
    display.displayNumber((unsigned short)7,decimal,false);

  }
 
 



 }
 
 void displayTemperatureAndHumidity(){
  

  if (oldTemp< 1 || oldHum<1){ // if this is the first time entering the method just print the instant temperature
    float temp= dht.readTemperature();
    int hum=dht.readHumidity();

    int integer= floor(temp); 
    int decimal= int((temp-integer)*100);
  

          display.displayNumber((unsigned short)0,integer,true);

                lc.setDigit(0,(int) 2, (int) decimal/10,false); // first digit

              lc.setChar(0,3,'C',false);
              display.displayNumber((unsigned short)5,hum,false);
              lc.setChar(0,7,'H',false);

    oldTemp=temp;
    oldHum=hum;
  }
  else
  {
    int integer= floor(oldTemp); 
    int decimal= int((oldTemp-integer)*100);
    int hum=dht.readHumidity();
    Serial.print("hum is ");
    Serial.println(hum);
    display.displayNumber((unsigned short)0,integer,true);
     Serial.print("decimal is ");
    Serial.println(decimal);
    lc.setDigit(0,2,decimal,false); 
    display.displayNumber((unsigned short)5,hum,false);

    long startTime=millis();
    float meanTemperature=dht.readTemperature();
    float meanHum=dht.readHumidity();
    int n=1;
    while(millis()-startTime<20000 && !checkButtons()){ // update every X seconds or when a button is pressed
      n++;
      meanTemperature = meanTemperature*(n-1)/n + meanTemperature/n; // iterative mean
      meanHum=meanHum*(n-1)/n + meanHum/n;
      delay(10);

    }
    Serial.println(meanTemperature);
    float temp= meanTemperature;
    oldHum=meanHum;
    oldTemp=temp;
    oldHum=hum;
    integer= floor(temp); 
    decimal= int((temp-integer)*100);
    display.displayNumber((unsigned short)0,integer,true);

      lc.setDigit(0,(int) 2, (int) decimal/10,false); // first digit

    lc.setChar(0,3,'C',false);
    display.displayNumber((unsigned short)5,hum,false);
    lc.setChar(0,7,'H',false);

 }}

void loop()
{
  rtc.refresh();
  unsigned short currentSecond = rtc.second();
  while(1){
  rtc.refresh();
  set_light_status();
  if (state == 0)
    { // initial state, normal clock
      // display current time
      if (rtc.second()==currentSecond)
        {
          // check state pin, luminosity, and other settings
          displayCurrentTime();
          checkButtons();
          
        }
      else
        {
        displayCurrentTime();
        }
        currentSecond=rtc.second();

    }
    else if (state==1){
      if (rtc.second()==currentSecond) // the second has not yet changed
        {
      displayCurrentTimeWithMillis(firstEnter); // first enter =1 by default
      firstEnter=0;
        }
      else
        {
        checkButtons();
        displayCurrentTime();
        firstEnter=1;
        displayCurrentTimeWithMillis(firstEnter); // first enter =1 by default
        firstEnter=0;
        currentSecond=rtc.second();
        }
        

    }
    else if (state==2){
        displayTemperatureAndTime();
        checkButtons();

        //todo separare parte intera e parte frazionaria con un punto 
    }
    else if (state==3){
      displayTemperatureAndHumidity();
      checkButtons();


    }

       else if (state==4){
     set_light();
     Serial.println(analogRead(2));


    }
    else if (state==42){ // enter time setting mode
        //printa i minuti sempre, 
        //tieni l'ora accesa per 1 secondo e spegnila per 0.2 secondi 
        //quando viene ripremuto il tasto centrale vai ai minuti
        // quando viene premuto su vai ora più, ricordando che a 24 devi cambiare.
        // quando viene premuto giù vai meno ricordano che non puoi andare a 0
        set_time();
    }



  }
  }





