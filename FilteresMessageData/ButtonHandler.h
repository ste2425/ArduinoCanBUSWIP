#ifndef ButtonHandler_h
#define ButtonHandler_h

#include "Arduino.h"

class ButtonHandler {
  public:
    ButtonHandler(int pin, int mode, int triggerOn);
    void initialise(void (*f)(void));
    void onLoop();
  private:
    int _pin;
    int _lastValue;
    void (*_onPress)(void);
    int _triggerOn;
    int _mode;
};

#endif
