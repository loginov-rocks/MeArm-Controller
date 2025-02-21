#include <Arduino.h>
#include <meArm.h>

#define SERIAL_BAUDRATE 9600

#define BASE_SERVO_PIN 9
#define SHOULDER_SERVO_PIN 10
#define ELBOW_SERVO_PIN 6
#define GRIPPER_SERVO_PIN 11

meArm arm(137, 47, -pi / 4, pi / 4,    // base
          133, 39, pi / 4, 3 * pi / 4, // shoulder
          145, 94, 0, -pi / 4,         // elbow
          80, 120, pi / 2, 0);         // gripper

String readBuffer = "";

String getInput();
boolean isCoordinateCommand(String);
void executeCoordinateCommand(String);

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    arm.begin(BASE_SERVO_PIN, SHOULDER_SERVO_PIN, ELBOW_SERVO_PIN, GRIPPER_SERVO_PIN);

    Serial.println("MeArm initialized");
}

void loop()
{
    String input = getInput();

    if (input.length() > 0)
    {
        if (isCoordinateCommand(input))
        {
            executeCoordinateCommand(input);
        }
        else if (input == "g")
        {
            Serial.println("Opening the gripper");
            arm.openGripper();
        }
        else if (input == "G")
        {
            Serial.println("Closing the gripper");
            arm.closeGripper();
        }
        else if (input == "O")
        {
            Serial.println("Going to the home point");
            arm.gotoPoint(0.0, 100.0, 50.0);
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

boolean isCoordinateCommand(String command)
{
    return (command[0] == 'X' || command[0] == 'Y' || command[0] == 'Z');
}

void executeCoordinateCommand(String command)
{
    float x = arm.getX(),
          y = arm.getY(),
          z = arm.getZ();

    // Get float value from command string after the first character.
    float val = command.substring(1).toFloat();

    // Determine what coordinate will be changed.
    switch (command[0])
    {
    case 'X':
        x = val;
        break;

    case 'Y':
        y = val;
        break;

    case 'Z':
        z = val;
        break;

    default:
        return;
    }

    Serial.print("Going to the point (");
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.print(z);
    Serial.print(")");

    if (arm.isReachable(x, y, z))
    {
        Serial.println();
    }
    else
    {
        Serial.println(", which is not reachable");
    }

    arm.gotoPoint(x, y, z);
}
