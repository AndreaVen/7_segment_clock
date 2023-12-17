#ifndef CLOCKRTC
#define CLOCKRTC
#include <Arduino.h>
#include "uRTCLib.h"
class RtClock {
  
  private:
   uRTCLib rtc;
  void init();

  public:
    RtClock();
    unsigned short getSecond();
    unsigned short getMinute();
    unsigned short getHour();
    void incrementHour();
    void incrementMinute();
    void decreaseHour();
    void decreaseMinute();

    unsigned short formatHour(short direction);
    unsigned short formatMinute(short direction);
    


};
#endif