#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <Arduino.h>
#include <Servo.h>

class Calibrator
{
public:
  ~Calibrator();
  void attachServo(int, const char *);
  void cycleServo();
  void setAngle(int);

private:
  struct ServoNode
  {
    int pin;
    char *name;
    Servo servo;
    ServoNode *next;

    ServoNode(int pin, const char *name) : pin(pin), next(nullptr)
    {
      strcpy(this->name, name);
      this->servo.attach(this->pin);
    }

    ~ServoNode()
    {
      delete[] this->name;
      servo.detach();
    }
  };

  ServoNode *headNode = nullptr;
  ServoNode *currentNode = nullptr;
  int nodesCount = 0;
};

#endif
