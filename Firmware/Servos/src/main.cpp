#include <Arduino.h>
#include <Servo.h>

#include "Button.h"

Button button(2);

Servo base, shoulder, elbow, claw;

// readSerial helpers.
const size_t readSerialBufferSize = 16; // Should be large enough to accommodate a command.
char readSerialBuffer[readSerialBufferSize];
size_t readSerialBufferIndex = 0;

char *readSerial();

struct ServoCommand
{
  Servo *servo;
  int angle;
};
ServoCommand *parseServoCommand(const char *);

void setup()
{
  Serial.begin(9600);

  base.attach(6);
  shoulder.attach(9);
  elbow.attach(10);
  claw.attach(11);

  Serial.println("Setup complete");
  Serial.println();
  Serial.println("Commands:");
  Serial.println("  <servo> <angle> - move a servo to an angle (from 0 to 180), for example:");
  Serial.println("    Base 10");
  Serial.println("    Shoulder 20");
  Serial.println("    Elbow 30");
  Serial.println("    Claw 45");
  Serial.println("  RESET - move all servos to default angle");
  Serial.println();
}

void loop()
{
  char *input = readSerial();

  if (input != nullptr)
  {
    Serial.print("< ");
    Serial.println(input);

    if (strcmp(input, "RESET") == 0)
    {
      Serial.println("Moving all servos to default angle...");

      base.write(90);
      shoulder.write(90);
      elbow.write(90);
      claw.write(25);

      Serial.println("Success!");
    }
    else
    {
      ServoCommand *command = parseServoCommand(input);

      if (command != nullptr)
      {
        command->servo->write(command->angle);
        Serial.println("Success!");
      }
    }

    Serial.println();
  }

  // React to button clicks.
  if (button.wasClicked())
  {
    Serial.println("Button clicked");
  }
}

/**
 * Reads characters from the serial input, ignoring leading and trimming trailing spaces. The function returns a
 * null-terminated string of characters received up to the first newline character ('\n'). If no data is available or
 * the input buffer is empty, it returns a null pointer.
 */
char *readSerial()
{
  while (Serial.available())
  {
    char inputChar = Serial.read();

    // Ignore leading spaces.
    if (readSerialBufferIndex == 0 && isspace(inputChar))
    {
      continue;
    }

    Serial.print(inputChar);

    // Exit when a new line is received.
    if (inputChar == '\n')
    {
      // Trim trailing spaces.
      while (readSerialBufferIndex > 0 && isspace(readSerialBuffer[readSerialBufferIndex - 1]))
      {
        readSerialBufferIndex--;
      }

      // Null-terminate the buffer and reset the index.
      readSerialBuffer[readSerialBufferIndex] = '\0';
      readSerialBufferIndex = 0;

      return readSerialBuffer;
    }

    if (readSerialBufferIndex < readSerialBufferSize - 1)
    {
      readSerialBuffer[readSerialBufferIndex++] = inputChar;
    }
  }

  return nullptr;
}

/**
 * Parses a command string to identify the servo (Base, Shoulder, Elbow, or Claw) and the desired angle (0-180
 * degrees). If valid, returns a pointer to the corresponding Servo object and the angle. If the command or angle is
 * invalid, it prints an error message and returns a `nullptr` and `-1`.
 */
ServoCommand *parseServoCommand(const char *command)
{
  static ServoCommand output;
  char name[16]; // Should be large enough to accommodate the servo name.
  int angle;

  if (sscanf(command, "%s %d", name, &angle) != 2)
  {
    Serial.println("ERROR: Unknown command!");

    return nullptr;
  }

  if (strcmp(name, "Base") == 0)
  {
    output.servo = &base;
  }
  else if (strcmp(name, "Shoulder") == 0)
  {
    output.servo = &shoulder;
  }
  else if (strcmp(name, "Elbow") == 0)
  {
    output.servo = &elbow;
  }
  else if (strcmp(name, "Claw") == 0)
  {
    output.servo = &claw;
  }
  else
  {
    Serial.println("ERROR: Unknown servo!");

    return nullptr;
  }

  if (angle < 0 || angle > 180)
  {
    Serial.println("ERROR: Invalid angle!");

    return nullptr;
  }

  output.angle = angle;

  Serial.print("Moving servo \"");
  Serial.print(name);
  Serial.print("\" to ");
  Serial.print(angle);
  Serial.println("°...");

  return &output;
}
