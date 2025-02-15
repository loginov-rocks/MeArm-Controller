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

void Calibrator::attachServo(int pin, const char *name)
{
  ServoNode *newNode = new ServoNode(pin, name);

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
  Serial.println(newNode->pin);
}

void Calibrator::cycleServo()
{
  if (this->nodesCount == 0)
  {
    Serial.println("[Calibrator] ERROR: No servos are attached to cycle through!");
    return;
  }

  this->currentNode = (this->currentNode == nullptr) ? this->headNode : this->currentNode->next;

  // Switch to disengaged state if all servos cycled through.
  if (this->currentNode == nullptr)
  {
    Serial.println("[Calibrator] Servos disengaged");
    return;
  }

  Serial.print("[Calibrator] Engaging servo \"");
  Serial.print(this->currentNode->name);
  Serial.print("\" attached to pin ");
  Serial.println(this->currentNode->pin);
}

void Calibrator::setAngle(int angle)
{
  if (this->currentNode == nullptr)
  {
    Serial.print("[Calibrator] Setting angle to ");
    Serial.print(angle);
    Serial.println("°");
    return;
  }

  Serial.print("[Calibrator] Moving servo \"");
  Serial.print(this->currentNode->name);
  Serial.print("\" to ");
  Serial.print(angle);
  Serial.println("°");

  this->currentNode->servo.write(angle);
}
