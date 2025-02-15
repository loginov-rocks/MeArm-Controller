#include "Button.h"

Button::Button(int pin) : pin(pin)
{
  pinMode(this->pin, INPUT);
}

void Button::setDebounceDelay(int debounceDelay)
{
  this->debounceDelay = debounceDelay;
}

bool Button::wasClicked()
{
  this->read();

  if (this->clicked)
  {
    this->clicked = false;

    return true;
  }

  return false;
}

void Button::read()
{
  bool currentReading = digitalRead(this->pin);
  unsigned long currentMillis = millis();

  if (currentReading != this->lastReading)
  {
    this->lastReadingChangeMillis = currentMillis;
  }

  if (currentMillis > (this->lastReadingChangeMillis + this->debounceDelay) &&
      currentReading != this->lastStableState)
  {
    this->lastStableState = currentReading;

    if (this->lastStableState == HIGH)
    {
      this->clicked = true;
    }
  }

  this->lastReading = currentReading;
}
