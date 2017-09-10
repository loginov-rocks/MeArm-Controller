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
    Serial.print("Base is at ");
    Serial.println(base.read());

    Serial.print("Shoulder is at ");
    Serial.println(shoulder.read());

    Serial.print("Elbow is at ");
    Serial.println(elbow.read());

    Serial.print("Claw is at ");
    Serial.println(claw.read());

    delay(1000);
}

