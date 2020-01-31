/**
 *Simple Buttons by ssbayanov
 * 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class SButton
{
public:
    SButton(uint8_t pin, int pressState = LOW, int pinMode = INPUT);
    void check();

    bool isPressed();
    bool isClicked();
    bool isLongClicked();
    uint8_t pin();

    void reset();

    void setClickDelay(uint64t delay);
    void setLongClickDelay(uint64t delay);

    SButton *next;
private:
    unsigned long buttonLastPressed;
    bool _isPressed;
    bool _isClicked;
    bool _isLongClicked;
    uint8_t _buttonPin;

    int _pressState;
    uint64_t _clickDelay;
    uint64_t _longClickDelay;
};


class SButtons
{
public:
    SButtons();

    void update();
    SButton *addButton(uint8_t pin, int pressState = LOW);
    void addButton(SButton *button);
    SButton *at(uint8t pos);
    SButton *atPin(int pin);

    int count();

    SButton *firstButton, *lastButton;

    uint8t _count;
};



#endif //BUTTON_H
