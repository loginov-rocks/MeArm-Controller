# Servo

A simple PlatformIO project for microcontrollers supporting the Arduino framework performing an initial sweep test and
then setting the servo to its default position at 90 degrees.

## Hardware Requirements

- Microcontroller supporting Arduino framework.
- Servo motor (I use SG90 connected to pin `6`):
  [scheme](https://docs.arduino.cc/tutorials/generic/basic-servo-control/).

## Usage

For [MeArm](https://github.com/MeArm/MeArm) or its clones, after setting the servo to `90` degrees and marking, set the
default position to `25` degrees.
