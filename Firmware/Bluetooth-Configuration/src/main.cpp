/**
 * Wire HM-10 bluetooth module to the Arduino Uno:
 * RX  - 3
 * TX  - 2
 * GND - GND
 * VCC - 5V
 *
 * If you use Arduino with additional UARTs, you can remove `<SoftwareSerial.h>` and replace `SerialBt` with the serial
 * you use (e.g. `Serial1`, `Serial2` or `Serial3` for Arduino Mega).
 */

#include <Arduino.h>
#include <SoftwareSerial.h>

#define PC_SERIAL_BAUDRATE 9600
#define BT_SERIAL_BAUDRATE 9600
#define BT_RX_PIN 3
#define BT_TX_PIN 2

SoftwareSerial SerialBt(BT_TX_PIN, BT_RX_PIN);

void setup()
{
    Serial.begin(PC_SERIAL_BAUDRATE);
    SerialBt.begin(BT_SERIAL_BAUDRATE);
}

void loop()
{
    if (SerialBt.available())
    {
        Serial.write(SerialBt.read());
    }

    if (Serial.available())
    {
        SerialBt.write(Serial.read());
    }
}
