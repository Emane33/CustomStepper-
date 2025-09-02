#include <Arduino.h>
#include <CustomStepper.h> // Include your library

// Create a stepper object with your pin connections
CustomStepper myStepper(8, 9, 10, 11);

void setup() {
  Serial.begin(9600);
  myStepper.setSpeed(5); // Set a slow speed for testing, should be between 1 to 10
  Serial.println("Testing CustomStepper Library!");
  delay(2000);

  // The library automatically enables the motor before movement
  // and disables it after. Manual enable/disable is also available.
  // myStepper.enable();  // Manually power the coils
  // myStepper.disable(); // Manually cut power to the coils
}

void loop() {
  // move in degree rather than steps
  Serial.println("Moving 90 degrees CW");
  myStepper.moveInDegree(90.0);
  delay(1000);

  //it support anti-clockwise movement
  Serial.println("Moving 90 degrees CCW");
  myStepper.moveInDegree(-90.0);
  delay(1000);

  // move in steps
  Serial.println("Moving to absolute position 1024");
  myStepper.moveToPosition(1024);
  delay(1000);

  // move to a specific angle
  Serial.println("Moving to absolute Angle 90");
  myStepper.moveToAngle(90);
  delay(1000);
 

  // Uncomment to test sweep (will run forever until reset)
  // moves between two angles over and over
  //Serial.println("Starting sweep between 0 and 180 degrees");
  //myStepper.sweep(0, 180, 10);
}
