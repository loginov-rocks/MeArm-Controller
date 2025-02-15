#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <Arduino.h>

class Potentiometer
{
public:
  Potentiometer(int);
  void setReadingDeviation(int);
  void setDebounceDelay(int);
  bool wasChanged();
  int getValue();

private:
  int pin;
  /*
   * Set the default reading deviation to 5. This means the servo will only respond to analog readings that change by
   * more than 5 units from the previous value, preventing unnecessary adjustments caused by small fluctuations in the
   * analog input. Since we map the 0-1023 analog readings to 0-180 degrees, this corresponds to about 1 degree of
   * servo movement per 5-6 units of analog input change.
   */
  int readingDeviation = 5;
  int debounceDelay = 20; // ms

  int lastReading;
  unsigned long lastReadingChangeMillis = 0;
  int lastStableValue;
  bool changed = false;

  void read();
};

#endif
