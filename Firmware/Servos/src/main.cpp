#include <Arduino.h>
#include <Servo.h>

Servo base, shoulder, elbow, claw;

String readBuffer = "";

String getInput();
boolean isServoCommand(String);
void executeServoCommand(String);

void setup()
{
  Serial.begin(9600);

  base.attach(6);
  shoulder.attach(9);
  elbow.attach(10);
  claw.attach(11);

  Serial.println("Setup complete");
}

void loop()
{
  String input = getInput();

  if (input.length() > 0)
  {
    if (isServoCommand(input))
    {
      executeServoCommand(input);
    }
    else if (input == "O")
    {
      Serial.println("All servos are rotating to 90 degrees");

      base.write(90);
      shoulder.write(90);
      elbow.write(90);
      claw.write(90);
    }
  }
}

String getInput()
{
  String input = "";

  while (Serial.available())
  {
    char c = Serial.read();

    if (c == '\n')
    {
      input = readBuffer;
      input.trim();
      readBuffer = "";
    }
    else if (c)
    {
      readBuffer += c;
    }
  }

  return input;
}

boolean isServoCommand(String command)
{
  return (command[0] == 'B' || command[0] == 'S' || command[0] == 'E' || command[0] == 'C');
}

void executeServoCommand(String command)
{
  Servo *servo;

  // Determine what servo will be moved.
  switch (command[0])
  {
  case 'B':
    servo = &base;
    Serial.print("Base");
    break;

  case 'S':
    servo = &shoulder;
    Serial.print("Shoulder");
    break;

  case 'E':
    servo = &elbow;
    Serial.print("Elbow");
    break;

  case 'C':
    servo = &claw;
    Serial.print("Claw");
    break;

  default:
    return;
  }

  // Get integer value from command string after the first character.
  int val = command.substring(1).toInt();

  // Validate the value.
  if (val < 0 || val > 180)
  {
    Serial.println(" is not rotating because of incorrect angle");
    return;
  }

  Serial.print(" is rotating to ");
  Serial.print(val);
  Serial.println(" degrees");

  servo->write(val);
}
