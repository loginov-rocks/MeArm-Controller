#include <Arduino.h>
#include <Servo.h>

Servo servo;

void setup()
{
  servo.attach(6);

  // Sweep back and forth.
  servo.write(0);
  delay(500);

  servo.write(180);
  delay(500);

  // Set default angle.
  servo.write(90);
}

void loop()
{
  // ¯\_(ツ)_/¯
}
