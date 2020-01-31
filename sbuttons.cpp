#include "sbuttons.h"

SButtons::SButtons() {
  firstButton = NULL;
  lastButton = NULL;
  _count = 0;
}

void SButtons::update()
{
  SButton *ptr = firstButton;
  while (ptr != NULL) {
    ptr->check();
    ptr = ptr->next;
  }
}

SButton *SButtons::addButton(uint8_t pin, int pressState)
{
  _count++;
  SButton *newButton = new SButton(pin, pressState);
  newButton->reset();
  if (firstButton == NULL) {
    firstButton = newButton;
  }
  else {
    lastButton->next = newButton;
  }

  lastButton = newButton;

  return newButton;
}

SButtons &SButtons::count()
{
  return _count;
}

SButton *SButtons::at(uint8t pos){
  SButton* tmp = firstButton;
  for(int i = 0; i < pos; i++){
      tmp = tmp->next;
      if(tmp == NULL)
        tmp = firstButton;
  }
  
  return *tmp;
}

SButton *SButtons::atPin(int pin){
  SButton* tmp = firstButton;
  for(int i = 0; i < _count; i++){
    if(tmp->pin() == pin)
      return *tmp

    tmp = tmp->next;
  }

  return *tmp;
}

SButton::SButton(uint8_t pin, int pressState, int pMode)
{
  pinMode(pin, pMode);
  buttonLastPressed = 0;
  _isPressed = false;
  _isClicked = false;
  _buttonPin = pin;

  _pressState = pressState;
  _clickDelay = 100;

  next = NULL;
  
}

void SButton::check()
{
  if (digitalRead(_buttonPin) == _pressState) {
    if (!_isPressed) {
      buttonLastPressed = millis();
      _isPressed = true;
    }
  }
  else {
    if (_isPressed) {
      if (millis() - buttonLastPressed > _clickDelay) {
        _isClicked = true;
        _isPressed = false;
      }
      if (millis() - buttonLastPressed > _clickDelay) {
        _isClicked = false;
        _isLongClicked = true;
        _isPressed = false;
      }
    }
  }
}

void SButton::reset()
{
  _isPressed = 0;
  _isClicked = false;
  _isPressed = false;
}

bool SButton::isPressed()
{
  return _isPressed;
}

bool SButton::isClicked()
{
  bool clicked = _isClicked;
  _isClicked = false;
  return clicked;
}

bool SButton::isLongClicked()
{
  bool clicked = _isLongClicked;
  _isLongClicked = false;
  return clicked;
}

uint8_t SButton::pin()
{
  return _buttonPin;
}

void SButton::setClickDelay(uint64t delay)
{
  _clickDelay = delay;
}

void SButton::setLongClickDelay(uint64t delay)
{
  _longClickDelay = delay;
}
