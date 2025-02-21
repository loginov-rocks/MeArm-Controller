#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin) : pin(pin)
{
  pinMode(this->pin, INPUT);

  // Initialize with the current reading to avoid unnecessary change triggers.
  this->lastReading = analogRead(this->pin);
  this->lastStableValue = this->lastReading;
}

void Potentiometer::setReadingDeviation(int readingDeviation)
{
  this->readingDeviation = readingDeviation;
}

void Potentiometer::setDebounceDelay(int debounceDelay)
{
  this->debounceDelay = debounceDelay;
}

bool Potentiometer::wasChanged()
{
  this->read();

  if (this->changed)
  {
    this->changed = false;

    return true;
  }

  return false;
}

int Potentiometer::getValue()
{
  return this->lastStableValue;
}

void Potentiometer::read()
{
  int currentReading = analogRead(this->pin);
  unsigned long currentMillis = millis();

  if (abs(currentReading - this->lastReading) >= this->readingDeviation)
  {
    this->lastReadingChangeMillis = currentMillis;
  }

  if (currentMillis > (this->lastReadingChangeMillis + this->debounceDelay) &&
      abs(currentReading - this->lastStableValue) >= this->readingDeviation)
  {
    this->lastStableValue = currentReading;
    this->changed = true;
  }

  this->lastReading = currentReading;
}
