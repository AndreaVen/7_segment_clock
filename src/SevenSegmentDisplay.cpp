#include "SevenSegmentDisplay.h"
SevenSegmentDisplay::SevenSegmentDisplay()
{
    this->number_of_light_mode = 15;
    this->light_status = 12;
}

void SevenSegmentDisplay::update()
{
}

void SevenSegmentDisplay::setLightStatus(int status)
{
    this->light_status = status;
}

void SevenSegmentDisplay::init()
{
    lc.shutdown(0, false);
    lc.setIntensity(0, light_status);
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
void SevenSegmentDisplay::displayNumber(unsigned short startingDigit, unsigned short number, bool dp)
{
    // todo prendere in input l'opzione dp così da poter usare il metodo anche con la temperatura ed umidità
    lc.setDigit(0, (int)startingDigit, (int)number / 10, false); // first digit
    if (startingDigit < 7)
    {
        lc.setDigit(0, (int)(startingDigit + 1), (int)(number % 10), dp); // second digit
    }
}

void SevenSegmentDisplay::displayLightStatus()
{
    displayNumber(1, light_status, false); // mostrar
}

void SevenSegmentDisplay::clear()
{
    lc.clearDisplay(0);
}

void SevenSegmentDisplay::setChar(int addr, int digit, char value, boolean dp)
{
    lc.setChar(0, 0, 'L', true);
}

void SevenSegmentDisplay::setLightAuto(int lux)
{
    // 3 levels of luminosity: high, normal and low.
    if (lux > 300 && lux < 600)
    {
        lc.setIntensity(0, 12);
    }
    if (lux < 150)
    {
        lc.setIntensity(0, 3);
    }
    if (lux > 700)
    {
        lc.setIntensity(0, 15);
    }
}