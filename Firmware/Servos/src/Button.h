#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
public:
  Button(int);
  void setDebounceDelay(int);
  bool wasClicked();

private:
  int pin;
  int debounceDelay = 50; // ms

  bool lastReading = LOW;
  unsigned long lastReadingChangeMillis = 0;
  bool lastStableState = LOW;
  bool clicked = false;

  void read();
};

#endif
