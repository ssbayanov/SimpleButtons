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

uint8_t SButtons::addAnalog(uint8_t pin, int minValue, int maxValue)
{
  AButton *newButton = new AButton( _count++, pin, minValue, maxValue);
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
    {
	  //Serial.println("Button find");
      return btn;      
	}

    btn = btn->next;
  }

  return NULL;
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

void SButtons::setPressCallback(uint8_t ID, void (*callback)())
{
  AbstractButton *btn = getButton(ID);
  if(btn)
    btn->setPressCallback(callback);
}

void SButtons::setReleaseCallback(uint8_t ID, void (*callback)())
{
  AbstractButton *btn = getButton(ID);
  if(btn)
    btn->setReleaseCallback(callback);
}

void SButtons::setClickCallback(uint8_t ID, void (*callback)())
{
  AbstractButton *btn = getButton(ID);
  if(btn)
    btn->setClickCallback(callback);
}

void SButtons::setLongClickCallback(uint8_t ID, void (*callback)())
{
  AbstractButton *btn = getButton(ID);
  if(btn)
    btn->setLongClickCallback(callback);
}

/**
 * 
 * Abstract buttons
 *  
 */

void AbstractButton::setPressCallback(void (*callback)())
{
_pressCallback = callback;
}

void AbstractButton::setReleaseCallback(void (*callback)())
{
_releaseCallback = callback;
}

void AbstractButton::setClickCallback(void (*callback)())
{
_clickCallback = callback;
}

void AbstractButton::setLongClickCallback(void (*callback)())
{
_longClickCallback = callback;
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

  _pressCallback = NULL;
  _releaseCallback = NULL;
  _clickCallback = NULL;
  _longClickCallback = NULL;

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

      if(_pressCallback != NULL)
        _pressCallback();
    }
  }
  else {
    if (_isPressed) {
      if (millis() - _buttonLastPressed < _pressDelay){
        reset();
        return;
      }
      
      if(_releaseCallback != NULL)
        _releaseCallback();

      if (millis() - _buttonLastPressed > _clickDelay) {
        _isClicked = true;
        _isPressed = false;

        if(_clickCallback != NULL)
          _clickCallback();
      }

      if (millis() - _buttonLastPressed > _longClickDelay) {
        _isClicked = false;
        _isLongClicked = true;
        _isPressed = false;
        if(_longClickCallback != NULL)
          _longClickCallback();
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

/**
 * 
 * Simple button mechanic or TTP223
 *  
 */

SButton::SButton(uint8_t ID, uint8_t pin, int pressState, int pMode) :
AbstractButton(ID)
{
  pinMode(pin, pMode);
  _buttonPin = pin;

  _pressState = pressState;
}

SButton::SButton(uint8_t pin, int pressState, int pMode) :
AbstractButton(0)
{
  pinMode(pin, pMode);
  _buttonPin = pin;

  _pressState = pressState;
}

bool SButton::isPressed()
{
	uint8_t value = digitalRead(_buttonPin);
	//if(value)
		//Serial.println(value);
  return value == _pressState;
}

uint8_t SButton::pin()
{
  return _buttonPin;
}

AButton::AButton(uint8_t ID, uint8_t pin, int minValue, int maxValue) :
AbstractButton(ID)
{
	_minValue = minValue;
    _maxValue = maxValue;

    _buttonPin = pin;
}

AButton::AButton(uint8_t pin, int minValue, int maxValue) :
AbstractButton(0)
{
	_minValue = minValue;
    _maxValue = maxValue;

    _buttonPin = pin;
}

bool AButton::isPressed()
{
  int value = analogRead(_buttonPin);
  int binvalue = (value <= _maxValue) && (value >= _minValue);
  //Serial.print(_maxValue);Serial.print(" ");Serial.print(_minValue);Serial.print(" ");Serial.print(value);Serial.print(" "); Serial.println(binvalue);
  return binvalue;
}

uint8_t AButton::pin()
{
  return _buttonPin;
}

#ifdef ESP32
TButton::TButton(uint8_t ID, uint8_t pin, int pressThreshold):
AbstractButton(ID)
{
  _touchPort = pinToTouchPort(pin);
  _pressThreshold = pressThreshold;
  _lastValue = touchRead(_touchPort);
}

TButton::TButton(uint8_t pin, int pressThreshold):
AbstractButton(0)
{
  _touchPort = pinToTouchPort(pin);`
  _pressThreshold = pressThreshold;
  _lastValue = touchRead(_touchPort);
}

uint8_t TButton::pinToTouchPort(uint8_t pin){
  switch(pin){
    case 4:
      return = T0;
      break;
    case 0:
      return = T1;
      break;
    case 2:
      return = T2;
      break;
    case 15:
      return = T3;
      break;
    case 13:
      return = T4;
      break;
    case 12:
      return = T5;
      break;
    case 14:
      return = T6;
      break;
    case 27:
      return = T7;
      break;
    case 33:
      return = T8;
      break;
    case 32:
      return = T9;
      break;
  }
  return -1;
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
