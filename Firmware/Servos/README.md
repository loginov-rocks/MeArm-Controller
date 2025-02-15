# Arduino Robotic Arm Controller

A robust, object-oriented implementation for controlling a 4-DOF robotic arm using Arduino. The system supports both
manual control through a potentiometer and button interface, as well as programmatic control via serial commands.

## Features

- Control 4 or more named servos (Base, Shoulder, Elbow, and Claw)
- Dual control interfaces:
  - Hardware interface with button selection and potentiometer control
  - Serial command interface for programmatic control
- Robust debouncing for both digital and analog inputs
- Dynamic servo management with named positions
- Error checking and validation for all inputs
- Detailed serial output for monitoring and debugging

## Hardware Requirements

- Arduino board (tested on Arduino Uno)
- 4 servo motors
- 1 pushbutton
- 1 potentiometer (10kΩ recommended)
- Appropriate power supply for servos

## Pin Configuration

- Button: Pin 2 (Digital)
- Potentiometer: Pin A0 (Analog)
- Servo Motors:
  - Base: Pin 6
  - Shoulder: Pin 9
  - Elbow: Pin 10
  - Claw: Pin 11

## Installation

1. Clone this repository
2. Open the project in Arduino IDE or Platform IO
3. Upload to your Arduino board

## Usage

### Hardware Control

1. Click the button to cycle through servo selection
2. Turn the potentiometer to adjust the selected servo's position
3. The system automatically maps the potentiometer's range (0-1023) to servo angles (0-180)

### Serial Commands

Connect to the Arduino's serial port (9600 baud) to send commands:

```
<servo> <angle> - Move a specific servo to an angle (0-180)
Examples:
  Base 90
  Shoulder 45
  Elbow 120
  Claw 30

RESET - Return all servos to their default positions
```

### Serial Output

The system provides detailed feedback through the serial interface:

- Startup configuration
- Servo attachment confirmation
- Current servo selections
- Position changes
- Error messages

## Classes

### Button

Handles digital input with debouncing for the servo selection button.

### Potentiometer

Manages analog input with smoothing and debouncing for position control.

### Calibrator

Manages servo motors with support for:

- Named servo positions
- Default angles
- Dynamic servo cycling
- Position control
- Group reset functionality

## Error Handling

The system includes comprehensive error checking for:

- Invalid servo angles
- Duplicate servo names or pins
- Invalid serial commands
- Buffer overflow protection
- Hardware configuration conflicts
