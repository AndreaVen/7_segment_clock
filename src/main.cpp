#include <Arduino.h>
#include <DS1302.h>
#include <LedControl.h>
#include <DHT.h>
void chek_and_set_time_down(short int );
void chek_and_set_time_up(short int );




DS1302 rtc(5, 3, 13); // RST, DAT, CLOCK
LedControl lc=LedControl(12,11,10,1); // din(miso),clock(mosi),csLoad

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT22
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
unsigned short state=0;
Time t;
unsigned short sec;
unsigned short min;
unsigned short hour;
short int leftButton=7;
short int centerButton=8;
short int rightButton=9;
bool debug=true;
int maxModes=2; // number of functionality of the clock
long currentMills;
unsigned long startingMillis=0;
short int central_button_counter=0; // once the hour and minutes has been set you can resume normal mode

bool firstEnter=1; // used in millis function to sync arduino millis and current second


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
  rtc.halt(false);
  rtc.writeProtect(false);
  Serial.begin(9600);
  dht.begin();  
  lc.shutdown(0,false);
  pinMode(leftButton,INPUT);
  pinMode(rightButton,INPUT);
  pinMode(centerButton,INPUT);
  //rtc.setTime(15,27,00); // set time for the first time
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
//todo prendere in input l'opzione dp così da poter usare il metodo anche con la temperatura ed umidità
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



void checkButtons()
{
  bool left = digitalRead(leftButton);
  bool right = digitalRead(rightButton);
  bool center = digitalRead(centerButton);
  if (right == 0)
  {
    print( (String)"Right");
    print((String)"old state= "+state);
  lc.clearDisplay(0);
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

  }
  else if (left == 0)
  {
    print((String)"left");
            print((String)"old state= "+state);

    lc.clearDisplay(0);

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

  }
  else if (center == 0)
  {
      print((String)"center");
      lc.clearDisplay(0);
      state=42;
      delay(300);
    // enter mode that changes the current time
  }
}
void blink_on(short int startingDigit){
  long unsigned int now=millis();
  unsigned int time_on=1000;
  while(millis()-now<time_on){
    t=rtc.getTime();
    sec=t.sec;
    min=t.min;
    hour=t.hour;
    displayNumber((unsigned short)0,hour); 
    displayNumber((unsigned short)2,min);
    chek_and_set_time_up(startingDigit);
    chek_and_set_time_down(startingDigit);
  }
}
int format_hour(int increment){
  if (increment>0){
    if (rtc.getTime().hour>22){ // it means now is 23:XX I have to set 00:XX
      return 0;
    }
    else{
      return rtc.getTime().hour+1;
    }
  }
  else{
        if (rtc.getTime().hour==0){ // it means now is 00:XX I have to set 23:XX
      return 23;
    }
    else{
      return rtc.getTime().hour-1;
    }
  }
}
int format_minutes(int increment){
if (increment>0){
    if (rtc.getTime().min==59){ // it means now is XX:59I have to set XX:00
      return 0;
    }
    else{
      return rtc.getTime().min+1;
    }
  }
  else{
        if (rtc.getTime().min==0){ // it means now is XX:00 I have to set XX:59
      return 59;
    }
    else{
      return rtc.getTime().min-1;
    }
  }


}


void chek_and_set_time_up(short int startingDigit){
  if (check_right()){
    t=rtc.getTime();
    min=t.min;
    hour=t.hour;
    sec=t.sec;
    if (startingDigit==0){ // setting hour
      int new_hour= format_hour(1);
      rtc.setTime(new_hour,min,sec); // set time hh:mm:ss
    }
    else if (startingDigit==1){ // setting minutes
      int new_minute=format_minutes(1);
      rtc.setTime(hour,new_minute,sec);
    }
    delay(300);
  }
  check_center();

}

void chek_and_set_time_down(short int startingDigit){
  if (check_left()){
    t=rtc.getTime();
    min=t.min;
    hour=t.hour;
    sec=t.sec;
    if (startingDigit==0){ // setting hour
      int new_hour= format_hour(-1);
      rtc.setTime(new_hour,min,sec); // set time hh:mm:ss
    }
    else if (startingDigit==1){ // setting minutes
      int new_minute=format_minutes(-1);
      rtc.setTime(hour,new_minute,sec);
    }

    delay(300);
    
  }
      check_center();

}


void  display_noting(short int startingDigit){
// display only the time NOT on the starting digit i.e if startingDigit=0 the function displays the minutes but not the hours
lc.clearDisplay(0);
long unsigned int now=millis();
unsigned int time_on=150;
while(millis()-now<time_on){
  t=rtc.getTime();
  sec=t.sec;
  min=t.min;
  hour=t.hour;
  if (startingDigit==0){
    displayNumber((unsigned short)2,min);
  }
  else if (startingDigit==1){
    displayNumber((unsigned short)0,hour); 
  }
  chek_and_set_time_up(startingDigit);
  chek_and_set_time_down(startingDigit);

}
}



void blink_off(short int startingDigit){
  display_noting(startingDigit);

}





void set_time()
{ 
  print("iniziato set_time");
  while(central_button_counter<2){
    print("ancora in set time causa valore counter=");
    Serial.println(central_button_counter);
    blink_on(central_button_counter);
    blink_off(central_button_counter);
  }
  print("uscendo da set time");
  state=1;
  central_button_counter=0; // once the hour and minutes has been set you can resume normal mode

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
    startingMillis=millis();
    displayNumber((unsigned short)6,(unsigned short)0);
    currentMills=millis();
  }
  else{
    currentMills=millis()-startingMillis; 
    displayNumber((unsigned short)6,(unsigned short)currentMills/10);
  }
  displayNumber((unsigned short)0,hour); 
  displayNumber((unsigned short)2,min);
  displayNumber((unsigned short)4,sec);
  checkButtons();
  
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
          displayCurrentTime();
          checkButtons();
          
        }
      else
        {
        displayCurrentTime();
        }
        currentSecond=rtc.getTime().sec;

    }
    else if (state==1){
      if (rtc.getTime().sec==currentSecond) // the second has not yet changed
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
        currentSecond=rtc.getTime().sec;
        }
        

    }
    else if (state==3){
        //display temp and humidity 
        float hum = dht.readHumidity();
        float temp= dht.readTemperature();
        //todo separare parte intera e parte frazionaria con un punto 
    }
    else if (state==43){ // enter time setting mode
        //printa i minuti sempre, 
        //tieni l'ora accesa per 1 secondo e spegnila per 0.2 secondi 
        //quando viene ripremuto il tasto centrale vai ai minuti
        // quando viene premuto su vai ora più, ricordando che a 24 devi cambiare.
        // quando viene premuto giù vai meno ricordano che non puoi andare a 0
        set_time();


    }



  }
  }





