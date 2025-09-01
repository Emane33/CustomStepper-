# CustomStepper Library

A lightweight, bare-metal Arduino/PlatformIO library for precise control of 28BYJ-48 stepper motors with ULN2003 driver boards.

## Features

- **Bare Metal Control:** Implements the full 8-step sequence for smooth operation.
- **Absolute Positioning:** Move to specific step counts or angles.
- **Speed Control:** Set speed in Revolutions Per Minute (RPM).
- **Power Management:** `enable()`/`disable()` functions to prevent motor heating.

## Installation

### PlatformIO
1. Open your project in PlatformIO.
2. Run the following command in the terminal:
```bash
pio pkg install --library "Emane33/CustomStepper"
```

## Hardware Wiring

| ULN2003 Pin | Arduino Pin |
|-------------|-------------|
| IN1         | 8           |
| IN2         | 9           |
| IN3         | 10          |
| IN4         | 11          |
| VCC         | 5V          |
| GND         | GND         |

## Quick Start

```cpp
#include <CustomStepper.h>

CustomStepper myStepper(8, 9, 10, 11); // Create motor object

void setup() {
  myStepper.setSpeed(10); // Set to 10 RPM
}

void loop() {
  myStepper.moveInDegree(360.0); // Spin one full revolution
  delay(1000);
}
```