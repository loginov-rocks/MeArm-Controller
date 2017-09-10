#include <Servo.h>

#define PC_SERIAL_BAUDRATE  115200

#define BASE_SERVO_PIN      9
#define SHOULDER_SERVO_PIN  10
#define ELBOW_SERVO_PIN     6
#define CLAW_SERVO_PIN      11

Servo base,
      shoulder,
      elbow,
      claw;

String inputBuffer = "";

void setup()
{
    Serial.begin(PC_SERIAL_BAUDRATE);

    base.attach(BASE_SERVO_PIN);
    shoulder.attach(SHOULDER_SERVO_PIN);
    elbow.attach(ELBOW_SERVO_PIN);
    claw.attach(CLAW_SERVO_PIN);
}

void loop()
{
    String input = getInput();

    if (input.length() > 0) {
        if (isServoCommand(input)) {
            executeServoCommand(input);
        }
    }
}

String getInput()
{
    String input = "";

    if (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            input = inputBuffer;
            inputBuffer = "";
        }
        else {
            inputBuffer += c;
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
    Servo servo;

    // Determine what servo will be moved
    switch (command[0]) {
        case 'B':
            servo = base;
            Serial.print("Base");
            break;
        case 'S':
            servo = shoulder;
            Serial.print("Shoulder");
            break;
        case 'E':
            servo = elbow;
            Serial.print("Elbow");
            break;
        case 'C':
            servo = claw;
            Serial.print("Claw");
            break;
        default:
            return;
    }

    // Get integer value from command string after the first character
    int val = command.substring(1).toInt();

    // Validate the value
    if (val < 0 || val > 180) {
        Serial.println(" is not rotating because of incorrect angle");
        return;
    }

    Serial.print(" is rotating to ");
    Serial.print(val);
    Serial.println(" degrees");

    servo.write(val);
}

