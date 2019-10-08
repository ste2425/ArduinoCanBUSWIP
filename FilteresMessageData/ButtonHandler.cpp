#include <arduino.h>
#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(int pin, int mode, int triggerOn) {
  _pin = pin;
  _mode = mode;
  _triggerOn = triggerOn;
}

void ButtonHandler::initialise(void (*f)(void)) {
  _onPress = f;
    
  pinMode(_pin, _mode);
}

void ButtonHandler::onLoop() {
  int value = digitalRead(_pin);

  if (value != _lastValue && value == _triggerOn)
    _onPress();

  _lastValue = value;
}
