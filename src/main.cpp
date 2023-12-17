#include <Arduino.h>
#include <DHT.h>
#include "SevenSegmentDisplay.h"
#include "RtClock.h"
void chek_and_set_time_down(short int );
void chek_and_set_time_up(short int );
void check_and_set_light_up();
void check_and_set_light_down();





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
unsigned short maxModes=4; // number of functionality of the clock
long currentMills;
unsigned long startingMillis=0;
short int central_button_counter=0; // once the hour and minutes has been set you can resume normal mode
float oldTemp=0;
int oldHum=0;
bool firstEnter=1; // used in millis function to sync arduino millis and current second

SevenSegmentDisplay display;
RtClock rtClock;

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
  // for example to set January 13 2022 at 12:56 you would call:
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
    min=rtClock.getMinute();
    hour=rtClock.getHour();
    display.displayNumber((unsigned short)0,hour,false); 
    display.displayNumber((unsigned short)2,min,true);
    chek_and_set_time_up(startingDigit);
    chek_and_set_time_down(startingDigit);
  }
}

void blink_light_on(short int startingDigit)
{
  long unsigned int now = millis();
  unsigned int time_on = 1000;
  while (millis() - now < time_on)
  {
    display.displayLightStatus();
    check_and_set_light_up();
    check_and_set_light_down();
  }
}

void chek_and_set_time_up(short int startingDigit)
{
  if (check_right())
  {
    if (startingDigit == 0)
    { // setting hour
      print("aumento ora");
      rtClock.incrementHour();
    }
    else if (startingDigit == 1)
    { // setting minutes
      print("aumento minuti");
      rtClock.incrementMinute();
    }
    delay(300);
  }
  check_center();
}

void updateLight()
{
  display.updateLight(analogRead(lux_pin));
}

void check_and_set_light_up()
{
  if (check_right())
  {
    display.increaseLight();
  }
  check_center();
}

void check_and_set_light_down()
{
  if (check_left())
  {
    display.decreaseLight();
  }

  check_center();
}

void chek_and_set_time_down(short int startingDigit)
{
  if (check_left())
  {
    if (startingDigit == 0)
    { // setting hour
      rtClock.decreaseHour();
    }
    else if (startingDigit == 1)
    { // setting minutes
      rtClock.decreaseMinute();
    }

    delay(300);
  }
  check_center();
}

void display_noting(short int startingDigit)
{
  // display only the time NOT on the starting digit i.e if startingDigit=0 the function displays the minutes but not the hours
  display.clear();
  long unsigned int now = millis();
  unsigned int time_on = 150;
  while (millis() - now < time_on)
  {
    min = rtClock.getMinute();
    hour = rtClock.getHour();
    if (startingDigit == 0)
    {
      display.displayNumber((unsigned short)2, min, true);
    }
    else if (startingDigit == 1)
    {
      display.displayNumber((unsigned short)0, hour, true);
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
    display.setChar(0,0,'L',true);
    print("ancora in set light causa valore counter=");
    Serial.println(central_button_counter);
    blink_light_on(central_button_counter);
    blink_light_off();
  }
  print("uscendo da set light");
  state=1;
  central_button_counter=0; // once the hour and minutes has been set you can resume normal mode

}

void displayCurrentTime()
{
  sec = rtClock.getSecond();
  min = rtClock.getMinute();
  hour = rtClock.getHour();
  display.displayNumber((unsigned short)0, hour, true);
  display.displayNumber((unsigned short)2, min, true);
  display.displayNumber((unsigned short)4, sec, false);
}

void displayCurrentTimeWithMillis(bool firstEnter)
{
  sec = rtClock.getSecond();
  min = rtClock.getMinute();
  hour = rtClock.getHour();

  if (firstEnter)
  {
    startingMillis = millis();
    display.displayNumber((unsigned short)6, (unsigned short)0, false);
    currentMills = millis();
  }
  else
  {
    currentMills = millis() - startingMillis;
    display.displayNumber((unsigned short)6, (unsigned short)currentMills / 10, false);
  }
  display.displayNumber((unsigned short)0, hour, true);
  display.displayNumber((unsigned short)2, min, true);
  display.displayNumber((unsigned short)4, sec, false);
  checkButtons();
}
void displayTemperatureAndTime()
{
  min = rtClock.getMinute();
  hour = rtClock.getHour();
  display.displayNumber((unsigned short)0, hour, true);
  display.displayNumber((unsigned short)2, min, false);

  if (oldTemp < 1)
  { // if this is the first time entering the method just print the instant temperature
    float temp = dht.readTemperature();
    int integer = floor(temp);
    int decimal = int((temp - integer) * 100);
    Serial.print("old: ");
    Serial.println(oldTemp);
    display.displayNumber((unsigned short)5, integer, true);
    display.displayNumber((unsigned short)7, decimal, false);
    oldTemp = temp;
  }
  else
  {
    int integer = floor(oldTemp);
    int decimal = int((oldTemp - integer) * 100);
    display.displayNumber((unsigned short)5, integer, true);
    display.displayNumber((unsigned short)7, decimal, false);
    long startTime = millis();
    float meanTemperature = dht.readTemperature();
    int n = 1;
    while (millis() - startTime < 20000 && !checkButtons())
    { // update every X seconds or when a button is pressed
      n++;
      meanTemperature = meanTemperature * (n - 1) / n + meanTemperature / n; // iterative mean
      delay(10);
    }
    Serial.println(meanTemperature);
    float temp = meanTemperature;
    oldTemp = temp;
    integer = floor(temp);
    decimal = int((temp - integer) * 100);
    display.displayNumber((unsigned short)5, integer, true);
    display.displayNumber((unsigned short)7, decimal, false);
  }
}

void displayTemperatureAndHumidity()
{

  if (oldTemp < 1 || oldHum < 1)
  { // if this is the first time entering the method just print the instant temperature
    float temp = dht.readTemperature();
    int hum = dht.readHumidity();
    int integer = floor(temp);
    int decimal = int((temp - integer) * 100);
    display.displayNumber((unsigned short)0, integer, true);
    display.displayNumber((int)2, (int)decimal / 10, false, false);
    display.setChar(0, 3, 'C', false);
    display.displayNumber((unsigned short)5, hum, false);
    display.setChar(0, 7, 'H', false);
    oldTemp = temp;
    oldHum = hum;
  }
  else
  {
    int integer = floor(oldTemp);
    int decimal = int((oldTemp - integer) * 100);
    int hum = dht.readHumidity();
    Serial.print("hum is ");
    Serial.println(hum);
    display.displayNumber((unsigned short)0, integer, true);
    Serial.print("decimal is ");
    Serial.println(decimal);
    display.displayNumber(2, decimal, false, false);
    display.displayNumber((unsigned short)5, hum, false, false);

    long startTime = millis();
    float meanTemperature = dht.readTemperature();
    float meanHum = dht.readHumidity();
    int n = 1;
    while (millis() - startTime < 20000 && !checkButtons())
    { // update every X seconds or when a button is pressed
      n++;
      meanTemperature = meanTemperature * (n - 1) / n + meanTemperature / n; // iterative mean
      meanHum = meanHum * (n - 1) / n + meanHum / n;
      delay(10);
    }
    Serial.println(meanTemperature);
    float temp = meanTemperature;
    oldHum = meanHum;
    oldTemp = temp;
    oldHum = hum;
    integer = floor(temp);
    decimal = int((temp - integer) * 100);
    display.displayNumber((unsigned short)0, integer, true);
    display.displayNumber((int)2, (int)decimal / 10, false); // first digit
    display.setChar(0, 3, 'C', false);
    display.displayNumber((unsigned short)5, hum, false);
    display.setChar(0, 7, 'H', false);
  }
}

void loop()
{
  unsigned short currentSecond = rtClock.getSecond();
  while(1){
  updateLight();
  if (state == 0)
    { // initial state, normal clock
      // display current time
      if (rtClock.getSecond()==currentSecond)
        {
          // check state pin, luminosity, and other settings
          displayCurrentTime();
          checkButtons();
          
        }
      else
        {
        displayCurrentTime();
        }
        currentSecond=rtClock.getSecond();

    }
    else if (state==1){
      if (rtClock.getSecond()==currentSecond) // the second has not yet changed
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
        currentSecond=rtClock.getSecond();
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
    /*
      blinks and set hour until center button is pushed, then repeats for minute.
      after the second push of the center button the time is set. 
    */
    else if (state==42){ // enter time setting mode
        set_time();
    }
  }
  }





