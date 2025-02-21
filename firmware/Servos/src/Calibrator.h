#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <Arduino.h>
#include <Servo.h>

class Calibrator
{
public:
  ~Calibrator();

  void attachServo(int, const char *, int);
  void cycleServo();
  void setAngle(int);
  void setAngle(const char *, int);
  void resetServos();

private:
  struct ServoNode
  {
    int pin;
    char *name;
    int defaultAngle;

    Servo servo;
    ServoNode *next;

    ServoNode(int, const char *, int);
    ~ServoNode();
  };

  ServoNode *headNode = nullptr;
  ServoNode *currentNode = nullptr;
  int nodesCount = 0;
};

#endif
