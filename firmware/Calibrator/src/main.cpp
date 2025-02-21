#include <Arduino.h>

#include "Button.h"
#include "Calibrator.h"
#include "Potentiometer.h"

Button button(2);
Calibrator calibrator;
Potentiometer potentiometer(A0);

// readSerial helpers.
const size_t readSerialBufferSize = 16; // Should be large enough to accommodate a command.
char readSerialBuffer[readSerialBufferSize];
size_t readSerialBufferIndex = 0;

char *readSerial();

void setup()
{
  Serial.begin(9600);

  Serial.println("Setting up...");
  Serial.println();

  calibrator.attachServo(6, "Base", 90);
  calibrator.attachServo(9, "Shoulder", 90);
  calibrator.attachServo(10, "Elbow", 90);
  calibrator.attachServo(11, "Claw", 25);

  Serial.println("Setup complete");
  Serial.println();
  Serial.println("Click the button to select one of the attached servos.");
  Serial.println("Turn the potentiometer to move the selected servo.");
  Serial.println();
  Serial.println("Serial commands supported:");
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
  // React to Serial input.
  char *input = readSerial();

  if (input != nullptr)
  {
    Serial.print("< ");
    Serial.println(input);

    if (strcmp(input, "RESET") == 0)
    {
      calibrator.resetServos();
    }
    else
    {
      char name[16]; // Should be large enough to accommodate the servo name.
      int angle;

      if (sscanf(input, "%s %d", name, &angle) == 2)
      {
        calibrator.setAngle(name, angle);
      }
      else
      {
        Serial.println("Unknown command");
      }
    }

    Serial.println();
  }

  // React to button clicks.
  if (button.wasClicked())
  {
    Serial.println("Button clicked");

    calibrator.cycleServo();

    Serial.println();
  }

  // React to potentiometer changes.
  if (potentiometer.wasChanged())
  {
    int value = potentiometer.getValue();

    Serial.print("Potentiometer value changed to ");
    Serial.println(value);

    // The potentiometer value (from 0 to 1023) is mapped to an angle (from 0 to 180) supported by the Servo library.
    calibrator.setAngle(map(value, 0, 1023, 0, 180));

    Serial.println();
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
