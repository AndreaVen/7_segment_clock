#ifndef SEVENSEGMENT
#define SEVENSEGMENT
#include <LedControl.h>
#include <Arduino.h>
class SevenSegmentDisplay {
  
  private:
    int light_status;
    int number_of_light_mode;
    LedControl lc=LedControl(12,11,10,1);
  public:
    SevenSegmentDisplay();
    void update();
    void setLightStatus(int status);
    void init();
    void displayNumber(unsigned short startingDigit,unsigned short number,bool dp);
    void displayLightStatus();
    void clear();
    void setChar(int addr, int digit, char value, boolean dp );

    void setLightAuto(int lux);

};
#endif