#include "RtClock.h"


RtClock::RtClock(){
    init();

}

void RtClock::init(){
  Wire.begin();
  URTCLIB_WIRE.begin();
  rtc.set_rtc_address(0x68);
}


unsigned short RtClock::getHour(){
    rtc.refresh();
    return rtc.hour();
}

unsigned short RtClock::getMinute()
{
    rtc.refresh();
    return rtc.minute();
}

unsigned short RtClock::getSecond()
{
    rtc.refresh();
    return rtc.second();
}

void RtClock::incrementHour()
{
    rtc.set(getSecond(), getMinute(), formatHour(1), 5, 13, 1, 22);
}

void RtClock::incrementMinute()
{
    rtc.set(getSecond(), formatMinute(1), getHour(), 5, 13, 1, 22);
}

void RtClock::decreaseHour(){
    rtc.set(getSecond(),getMinute(), formatHour(-1), 5, 13, 1, 22);
}

void RtClock::decreaseMinute(){
    rtc.set(getSecond(), formatMinute(-1),getHour() , 5, 13, 1, 22);
}


/*
this method formats the current time preventing the setting of strange hours e.g 25:74
*/
unsigned short RtClock::formatHour(short direction)
{

    if (direction > 0)
    {
        if (getHour() > 22)
        { // it means now is 23:XX I have to set 00:XX
            return 0;
        }
        else
        {
            return getHour() + 1;
        }
    }
    else
    {
        if (getHour() == 0)
        { // it means now is 00:XX I have to set 23:XX
            return 23;
        }
        else
        {
            return getHour() - 1;
        }
    }
}

unsigned short RtClock::formatMinute(short direction)
{
    if (direction > 0)
    {
        if (getMinute() == 59)
        { // it means now is XX:59I have to set XX:00
            return 0;
        }
        else
        {
            return getMinute() + 1;
        }
    }
    else
    {
        if (getMinute() == 0)
        { // it means now is XX:00 I have to set XX:59
            return 59;
        }
        else
        {
            return getMinute() - 1;
        }
    }
}