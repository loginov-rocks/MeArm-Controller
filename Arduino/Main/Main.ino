#include <Servo.h>
#include <meArm.h>

#define PC_SERIAL_BAUDRATE  115200

#define BASE_SERVO_PIN      9
#define SHOULDER_SERVO_PIN  10
#define ELBOW_SERVO_PIN     6
#define GRIPPER_SERVO_PIN   11

typedef enum {
    SERVOS,
    COORDINATES
} mode_t;

Servo base,
      shoulder,
      elbow,
      gripper;

meArm arm(137, 47,  -pi / 4, pi / 4,     // base
          133, 39,  pi / 4,  3 * pi / 4, // shoulder
          145, 94,  0,       -pi / 4,    // elbow
          80,  120, pi / 2,  0);         // gripper

String inputBuffer = "";

mode_t mode = SERVOS;

void setup()
{
    Serial.begin(PC_SERIAL_BAUDRATE);

    switch (mode) {
        case COORDINATES:
            setupCoordinatesMode();
            break;
        default:
            setupServosMode();
            break;
    }

    Serial.println("MeArm initialized");
}

void loop()
{
    String input = getInput();

    if (input.length() > 0) {
        if (isServoCommand(input)) {
            switchMode(SERVOS);
            executeServoCommand(input);
        }
        else if (isCoordinateCommand(input)) {
            switchMode(COORDINATES);
            executeCoordinateCommand(input);
        }
        else if (input == "c") {
            switchMode(COORDINATES);
            Serial.println("Opening the gripper");
            arm.openGripper();
        }
        else if (input == "C") {
            switchMode(COORDINATES);
            Serial.println("Closing the gripper");
            arm.closeGripper();
        }
        else if (input == "D") {
            Serial.println(getCurrentModeData());
        }
    }
}

void setupServosMode()
{
    base.attach(BASE_SERVO_PIN);
    shoulder.attach(SHOULDER_SERVO_PIN);
    elbow.attach(ELBOW_SERVO_PIN);
    gripper.attach(GRIPPER_SERVO_PIN);
}

void disarmServosMode()
{
    base.detach();
    shoulder.detach();
    elbow.detach();
    gripper.detach();
}

void setupCoordinatesMode()
{
    arm.begin(BASE_SERVO_PIN, SHOULDER_SERVO_PIN, ELBOW_SERVO_PIN, GRIPPER_SERVO_PIN);
}

void disarmCoordinatesMode()
{
    arm.end();
}

boolean switchMode(mode_t targetMode)
{
    if (targetMode == mode) {
        return false;
    }

    switch (mode) {
        case SERVOS:
            disarmServosMode();
            break;
        case COORDINATES:
            disarmCoordinatesMode();
            break;
        default:
            return false;
    }

    switch (targetMode) {
        case SERVOS:
            Serial.println("Switching to the servos mode");
            setupServosMode();
            break;
        case COORDINATES:
            Serial.println("Switching to the coordinates mode");
            setupCoordinatesMode();
            break;
        default:
            return false;
    }

    mode = targetMode;

    return true;
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
    return (command[0] == 'B' || command[0] == 'S' || command[0] == 'E' || command[0] == 'G');
}

boolean isCoordinateCommand(String command)
{
    return (command[0] == 'X' || command[0] == 'Y' || command[0] == 'Z');
}

void executeServoCommand(String command)
{
    Servo *servo;

    // Determine what servo will be moved
    switch (command[0]) {
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
        case 'G':
            servo = &gripper;
            Serial.print("Gripper");
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

    servo->write(val);
}

void executeCoordinateCommand(String command)
{
    float x = arm.getX(),
          y = arm.getY(),
          z = arm.getZ();

    // Get float value from command string after the first character
    float val = command.substring(1).toFloat();

    // Determine what coordinate will be changed
    switch (command[0]) {
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

    if (arm.isReachable(x, y, z)) {
        Serial.println();
    }
    else {
        Serial.println(", which is not reachable");
    }

    arm.gotoPoint(x, y, z);
}

String getCurrentModeData()
{
    switch (mode) {
        case SERVOS:
            return "DATA:MODE=SERVOS," + getServosModeData();
        case COORDINATES:
            return "DATA:MODE=COORDINATES," + getCoordinatesModeData();
        default:
            return "DATA:MODE=UNKNOWN";
    }
}

String getServosModeData()
{
    String data;

    data += "B=";
    data += base.read();
    data += ",S=";
    data += shoulder.read();
    data += ",E=";
    data += elbow.read();
    data += ",G=";
    data += gripper.read();

    return data;
}

String getCoordinatesModeData()
{
    String data;

    data += "X=";
    data += arm.getX();
    data += ",Y=";
    data += arm.getY();
    data += ",Z=";
    data += arm.getZ();

    return data;
}

