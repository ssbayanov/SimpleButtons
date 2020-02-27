#include "sbuttons.h"

SButtons::SButtons() {
  firstButton = NULL;
  lastButton = NULL;
  _count = 0;
}

void SButtons::update()
{
  AbstractButton *ptr = firstButton;
 
  while (ptr != NULL) {
    ptr->check();
    ptr = ptr->next;
  }
}


uint8_t SButtons::addButton(uint8_t pin, int pressState, int pMode)
{  
  SButton *newButton = new SButton( _count++, pin, pressState, pMode);
  insert(newButton);
  return newButton->id();
}

uint8_t SButtons::addAnalog(uint8_t pin, int maxValue, int lowValue)
{
  AButton *newButton = new AButton( _count++, pin, maxValue, lowValue);
  insert(newButton);
  return newButton->id();
}

#ifdef ESP32
uint8_t SButtons::addTouch(uint8_t pin, int pressThreshold)
{
  TButton *newButton = new TButton( _count++, pin, pressThreshold);
  insert(newButton);
  return newButton->id();
}
#endif

uint8_t SButtons::count()
{
  return _count;
}

AbstractButton *SButtons::getButton(uint8_t ID){
  AbstractButton* btn = firstButton;
  for(int i = 0; i < _count; i++){
    if(btn->id() == ID)
      return btn;

    btn = btn->next;
  }

  return NULL;
}

bool SButtons::isPressed(uint8_t ID){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->isPressed();
  return false;
}

bool SButtons::isClicked(uint8_t ID){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->isClicked();
  return false;
}
bool SButtons::isLongClicked(uint8_t ID){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->isLongClicked();
  return false;
}

void SButtons::reset(uint8_t ID){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->reset();
}

void SButtons::setClickDelay(uint8_t ID, uint64_t delay){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->setClickDelay(delay);
}

void SButtons::setLongClickDelay(uint8_t ID, uint64_t delay){
  AbstractButton *btn = getButton(ID);
  if(btn)
    return btn->setLongClickDelay(delay);
}

void SButtons::insert(AbstractButton *btn)
{

  if (firstButton == NULL) {
    firstButton = btn;
  }
  else {
    lastButton->next = btn;
  }

  lastButton = btn;
}

AbstractButton::AbstractButton(uint8_t ID)
{
  _buttonLastPressed = 0;
  _isPressed = false;
  _isClicked = false;
  _ID = ID;
  _pressDelay = 100;
  _clickDelay = 300;
  _longClickDelay = 750;

  next = NULL;
}

bool AbstractButton::isPressed(){
  return false;
}

void AbstractButton::check()
{
  if (isPressed()) {
    if (!_isPressed) {
      _buttonLastPressed = millis();
      _isPressed = true;
    }
  }
  else {
    if (_isPressed) {
      if (millis() - _buttonLastPressed < _pressDelay){
        reset();
        return;
      }
        
      if (millis() - _buttonLastPressed > _clickDelay) {
        _isClicked = true;
        _isPressed = false;
      }
      if (millis() - _buttonLastPressed > _longClickDelay) {
        _isClicked = false;
        _isLongClicked = true;
        _isPressed = false;
      }
    }
  }
}

void AbstractButton::reset()
{
  _isPressed = 0;
  _isClicked = false;
  _isPressed = false;
}

bool AbstractButton::isClicked()
{
  bool clicked = _isClicked;
  _isClicked = false;
  return clicked;
}

bool AbstractButton::isLongClicked()
{
  bool clicked = _isLongClicked;
  _isLongClicked = false;
  return clicked;
}

uint8_t AbstractButton::id()
{
  return _ID;
}

void AbstractButton::setClickDelay(uint64_t delay)
{
  _clickDelay = delay;
}

void AbstractButton::setLongClickDelay(uint64_t delay)
{
  _longClickDelay = delay;
}

SButton::SButton(uint8_t ID, uint8_t pin, int pressState, int pMode) :
AbstractButton(ID)
{
  pinMode(pin, pMode);

  _pressState = pressState;
}

bool SButton::isPressed()
{
  return digitalRead(_buttonPin) == _pressState;
}

uint8_t SButton::pin()
{
  return _buttonPin;
}

AButton::AButton(uint8_t ID, uint8_t pin, int maxValue, int minValue) :
AbstractButton(ID)
{
    _maxValue = maxValue;
    _minValue = minValue;

    _buttonPin = pin;
}

bool AButton::isPressed()
{
  int value = analogRead(_buttonPin);
  return (value < _maxValue) && (value > _minValue);
}

uint8_t AButton::pin()
{
  return _buttonPin;
}

#ifdef ESP32
TButton::TButton(uint8_t ID, uint8_t pin, int pressThreshold):
AbstractButton(ID)
{
  switch(pin){
    case 4:
      _touchPort = T0;
      break;
    case 0:
      _touchPort = T1;
      break;
    case 2:
      _touchPort = T2;
      break;
    case 15:
      _touchPort = T3;
      break;
    case 13:
      _touchPort = T4;
      break;
    case 12:
      _touchPort = T5;
      break;
    case 14:
      _touchPort = T6;
      break;
    case 27:
      _touchPort = T7;
      break;
    case 33:
      _touchPort = T8;
      break;
    case 32:
      _touchPort = T9;
      break;
  }
  _pressThreshold = pressThreshold;
  _lastValue = touchRead(_touchPort);
}

bool TButton::isPressed()
{
  
  uint8_t newValue = touchRead(_touchPort);
  float delta = 0.01 * abs(30 - abs(newValue - _lastValue));
  //if(delta > 1)
  _lastValue = (newValue * delta) + (_lastValue * (1 - delta));


  Serial.print("Touch filtred value: "); Serial.println(_lastValue);
  return (_lastValue < _pressThreshold);
}

uint8_t TButton::pin()
{
  return _buttonPin;
}
#endif