/**
 *Simple Buttons by ssbayanov
 * 
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

// Abstract button class
class AbstractButton
{
public:
    AbstractButton(uint8_t ID);
    void check();
    virtual bool isPressed();
    bool isClicked();
    bool isLongClicked();

    void reset();

    void setClickDelay(uint64_t delay);
    void setLongClickDelay(uint64_t delay);

    uint8_t id();

    AbstractButton *next;
    
protected:
    uint8_t _ID;

    unsigned long _buttonLastPressed;
    bool _isPressed;
    bool _isClicked;
    bool _isLongClicked;
    

    uint64_t _pressDelay;
    uint64_t _clickDelay;
    uint64_t _longClickDelay;
};

// Classic button or touch button on TTP223
class SButton : public AbstractButton
{
public:
    SButton(uint8_t ID, uint8_t pin, int pressState = LOW, int pMode = INPUT);

    bool isPressed();

    uint8_t pin();

private:
    
    uint8_t _pressState;

    uint8_t _buttonPin;
};

// Class of analog button
class AButton : public AbstractButton
{
public:
    AButton(uint8_t ID, uint8_t pin, int minValue = 0, int maxValue = 1024);

    bool isPressed();

    uint8_t pin();

private:
    
    int _maxValue;
    int _minValue;

    uint8_t _buttonPin;
};

#ifdef ESP32
// Touch buttons for ESP32
class TButton : public AbstractButton
{
public:
    TButton(uint8_t ID, uint8_t pin, int pressThreshold);
    bool isPressed();

    uint8_t pin();

private:    
    uint8_t _pressThreshold;
    uint8_t _touchPort;
    uint8_t _lastValue;

    uint8_t _buttonPin;
};
#endif

class SButtons
{
public:
    SButtons();

    void update();

    uint8_t addButton(uint8_t pin, int pressState = LOW, int pMode = INPUT_PULLUP);

    uint8_t addAnalog(uint8_t pin, int minValue = 0, int maxValue = 1024);
    

    #ifdef ESP32
    uint8_t addTouch(uint8_t pin, int pressThreshold = 50);
    #endif

    uint8_t count();

    bool isPressed(uint8_t pin);
    bool isClicked(uint8_t pin);
    bool isLongClicked(uint8_t pin);

    void reset(uint8_t pin);

    void setClickDelay(uint8_t pin, uint64_t delay);
    void setLongClickDelay(uint8_t pin, uint64_t delay);

    AbstractButton *getButton(uint8_t pin);

private:
    AbstractButton *firstButton, *lastButton;
    uint8_t _count;
    void insert(AbstractButton *btn);
};



#endif //BUTTON_H
