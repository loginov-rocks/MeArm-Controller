#include "Calibrator.h"

Calibrator::~Calibrator()
{
  while (this->headNode != nullptr)
  {
    ServoNode *tempNode = this->headNode;
    this->headNode = tempNode->next;
    delete tempNode;
  }
}

void Calibrator::attachServo(int pin, const char *name, int defaultAngle)
{
  ServoNode *tempNode = this->headNode;

  while (tempNode != nullptr)
  {
    if (tempNode->pin == pin)
    {
      Serial.print("[Calibrator] ERROR: Servo \"");
      Serial.print(tempNode->name);
      Serial.print("\" is already attached to pin ");
      Serial.print(tempNode->pin);
      Serial.println("!");

      return;
    }

    if (strcmp(tempNode->name, name) == 0)
    {
      Serial.print("[Calibrator] ERROR: Servo named \"");
      Serial.print(tempNode->name);
      Serial.println("\" is already attached!");

      return;
    }

    tempNode = tempNode->next;
  }

  if (defaultAngle < 0 || defaultAngle > 180)
  {
    Serial.print("[Calibrator] ERROR: Invalid default angle ");
    Serial.print(defaultAngle);
    Serial.print("° for servo \"");
    Serial.print(name);
    Serial.println("\"!");

    return;
  }

  ServoNode *newNode = new ServoNode(pin, name, defaultAngle);

  if (this->headNode == nullptr)
  {
    this->headNode = newNode;
  }
  else
  {
    ServoNode *lastNode = this->headNode;

    while (lastNode->next != nullptr)
    {
      lastNode = lastNode->next;
    }

    lastNode->next = newNode;
  }

  this->nodesCount++;

  Serial.print("[Calibrator] Servo \"");
  Serial.print(newNode->name);
  Serial.print("\" is attached to pin ");
  Serial.print(newNode->pin);
  Serial.print(" with a default angle of ");
  Serial.print(newNode->defaultAngle);
  Serial.println("°");
}

void Calibrator::cycleServo()
{
  if (this->nodesCount == 0)
  {
    Serial.println("[Calibrator] ERROR: No servos are attached to cycle through!");

    return;
  }

  this->currentNode = (this->currentNode == nullptr) ? this->headNode : this->currentNode->next;

  // Report disengaged state if all servos are cycled through.
  if (this->currentNode == nullptr)
  {
    Serial.println("[Calibrator] Servos disengaged");

    return;
  }

  Serial.print("[Calibrator] Servo \"");
  Serial.print(this->currentNode->name);
  Serial.print("\" attached to pin ");
  Serial.print(this->currentNode->pin);
  Serial.println(" engaged");
}

void Calibrator::setAngle(int angle)
{
  if (angle < 0 || angle > 180)
  {
    Serial.print("[Calibrator] ERROR: Invalid angle ");
    Serial.print(angle);
    Serial.println("°!");

    return;
  }

  if (this->currentNode == nullptr)
  {
    Serial.print("[Calibrator] Angle set to ");
    Serial.print(angle);
    Serial.println("°");

    return;
  }

  this->currentNode->servo.write(angle);

  Serial.print("[Calibrator] Servo \"");
  Serial.print(this->currentNode->name);
  Serial.print("\" set to ");
  Serial.print(angle);
  Serial.println("°");
}

void Calibrator::setAngle(const char *name, int angle)
{
  if (angle < 0 || angle > 180)
  {
    Serial.print("[Calibrator] ERROR: Invalid angle ");
    Serial.print(angle);
    Serial.println("°!");

    return;
  }

  ServoNode *tempNode = this->headNode;

  while (tempNode != nullptr)
  {
    if (strcmp(tempNode->name, name) == 0)
    {
      tempNode->servo.write(angle);

      Serial.print("[Calibrator] Servo \"");
      Serial.print(tempNode->name);
      Serial.print("\" set to ");
      Serial.print(angle);
      Serial.println("°");

      return;
    }

    tempNode = tempNode->next;
  }

  Serial.print("[Calibrator] ERROR: Servo named \"");
  Serial.print(name);
  Serial.println("\" not found!");
}

void Calibrator::resetServos()
{
  Serial.println("[Calibrator] Resetting servos to default angles...");

  ServoNode *tempNode = this->headNode;

  while (tempNode != nullptr)
  {
    tempNode->servo.write(tempNode->defaultAngle);

    Serial.print("[Calibrator]   Servo \"");
    Serial.print(tempNode->name);
    Serial.print("\" set to default angle ");
    Serial.print(tempNode->defaultAngle);
    Serial.println("°");

    tempNode = tempNode->next;
  }
}

Calibrator::ServoNode::ServoNode(int pin, const char *name, int defaultAngle)
    : pin(pin), defaultAngle(defaultAngle), next(nullptr)
{
  this->name = new char[strlen(name) + 1];
  strcpy(this->name, name);

  // Set the initial angle before attaching the servo; otherwise, it will move to 90 degrees by default when attached
  // (implemented in the Servo library).
  this->servo.write(defaultAngle);
  this->servo.attach(this->pin);
}

Calibrator::ServoNode::~ServoNode()
{
  delete[] this->name;
  servo.detach();
}
