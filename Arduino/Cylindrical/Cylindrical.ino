#include <Servo.h>
#include <meArm.h>

#define PC_SERIAL_BAUDRATE  115200

#define BASE_SERVO_PIN      9
#define SHOULDER_SERVO_PIN  10
#define ELBOW_SERVO_PIN     6
#define GRIPPER_SERVO_PIN   11

meArm arm(137, 47,  -pi / 4, pi / 4,     // base
          133, 39,  pi / 4,  3 * pi / 4, // shoulder
          145, 94,  0,       -pi / 4,    // elbow
          80,  120, pi / 2,  0);         // gripper

String inputBuffer = "";

void setup()
{
    Serial.begin(PC_SERIAL_BAUDRATE);

    arm.begin(BASE_SERVO_PIN, SHOULDER_SERVO_PIN, ELBOW_SERVO_PIN, GRIPPER_SERVO_PIN);

    Serial.println("MeArm initialized");
}

void loop()
{
    String input = getInput();

    if (input.length() > 0) {
        if (isCylindricalCommand(input)) {
            executeCylindricalCommand(input);
        }
        else if (input == "g") {
            Serial.println("Opening the gripper");
            arm.openGripper();
        }
        else if (input == "G") {
            Serial.println("Closing the gripper");
            arm.closeGripper();
        }
        else if (input == "O") {
            Serial.println("Going to the home point");
            arm.gotoPointCylinder(0.0, 100.0, 50.0);
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

boolean isCylindricalCommand(String command)
{
    return (command[0] == 'R' || command[0] == 'P' || command[0] == 'Z');
}

void executeCylindricalCommand(String command)
{
    float rho = arm.getR(),
          phi = arm.getTheta(),
          z   = arm.getZ();

    // Get float value from command string after the first character
    float val = command.substring(1).toFloat();

    // Determine what coordinate will be changed
    switch (command[0]) {
        case 'R':
            rho = val;
            break;
        case 'P':
            phi = val;
            break;
        case 'Z':
            z = val;
            break;
        default:
            return;
    }

    Serial.print("Going to the cylindrical point (");
    Serial.print(rho);
    Serial.print(", ");
    Serial.print(phi);
    Serial.print(", ");
    Serial.print(z);
    Serial.println(")");

    arm.gotoPointCylinder(phi, rho, z);
}

