#include <Arduino.h>
#include <math.h>
#include "CustomStepper.h"

// Constructor: Initialize pins and set as outputs
CustomStepper::CustomStepper(int pin1, int pin2, int pin3, int pin4) {
  _pin1 = pin1;
  _pin2 = pin2;
  _pin3 = pin3;
  _pin4 = pin4;
  
  pinMode(_pin1, OUTPUT);
  pinMode(_pin2, OUTPUT);
  pinMode(_pin3, OUTPUT);
  pinMode(_pin4, OUTPUT);

  _absolutePosition = 0;
  _sequenceIndex = 0;
  _currentAngle = 0.0;
  _delayBetweenSteps = 0;
}

void CustomStepper::performStep(int stepNumber) {
  digitalWrite(_pin1, _stepSequence[stepNumber][0]);
  digitalWrite(_pin2, _stepSequence[stepNumber][1]);
  digitalWrite(_pin3, _stepSequence[stepNumber][2]);
  digitalWrite(_pin4, _stepSequence[stepNumber][3]);
}

void CustomStepper::disable()
{
  digitalWrite(_pin1, LOW);
  digitalWrite(_pin2, LOW);
  digitalWrite(_pin3, LOW);
  digitalWrite(_pin4, LOW);
}

void CustomStepper::enable()
{
  int lastIndexUsed = _sequenceIndex;
  performStep(lastIndexUsed);
}

void CustomStepper::moveToPosition(int targetPosition)
{
  enable();
  Serial.print("moveToPosition called with target: ");
  Serial.println(targetPosition);
  // 1. Calculate how many steps we need to move
  //    stepsToMove = targetPosition - absolutePosition;
  int stepstomove = targetPosition - _absolutePosition;
  // 2. Figure out the direction based on the sign of stepsToMove
  int side = (stepstomove > 0) ? 1 : -1;
  stepstomove = abs(stepstomove);

  for (int i = 0; i < stepstomove; i++)
  {
    performStep(_sequenceIndex);
    _sequenceIndex += side;
    if (_sequenceIndex > 7)
      _sequenceIndex = 0;
    if (_sequenceIndex < 0)
      _sequenceIndex = 7;
    _absolutePosition += side;
    delayMicroseconds(_delayBetweenSteps);
  }
  disable();
}
void CustomStepper::moveInDegree(float degree)
{
  enable();
  // 4096 steps do a full turn, which is 360 degrees
  long totalStepsToMove = degree * _stepsPerDegree; // This can be positive or negative

  // Determine the direction from the original 'degree' input
  int direction = (degree >= 0) ? 1 : -1;  // POSITIVE degree -> POSITIVE direction (1)
  int stepsToMove = abs(totalStepsToMove); // Get the number of steps as a positive value

  Serial.print("Moving ");
  Serial.print(stepsToMove);
  Serial.print(" steps ");
  Serial.println(direction == 1 ? "CLOCKWISE" : "COUNTER-CLOCKWISE");

  for (int i = 0; i < stepsToMove; i++)
  {
    performStep(_sequenceIndex);
    _sequenceIndex += direction;
    if (_sequenceIndex > 7)
      _sequenceIndex = 0;
    if (_sequenceIndex < 0)
      _sequenceIndex = 7;
    _absolutePosition += direction; // Keep track of overall position
    delayMicroseconds(_delayBetweenSteps);
  }
  _currentAngle += degree; // Update the tracked angle after the move
  disable();
}

void CustomStepper::setSpeed(float rpm) //should be between 1 to 15
{
  // rpm = revolution per minute
  // motor has 4096 step/revolution
    if (rpm <= 0.0) {
    Serial.println("Error: RPM must be greater than 0. Using default 1 RPM.");
    rpm = 1.0; // Set a safe default value
  }
  // Calculate delay between steps in MICROseconds
  _delayBetweenSteps = (60.0 * 1000000.0) / (rpm * 4096.0); // Use floats for calculation
}

void CustomStepper::moveToAngle(float targetAngle)
{
  float difference = targetAngle - _currentAngle;
  // Normalize the difference to be between -180 and 180 degrees
  difference = fmod(difference, 360.0); // fmod gets the remainder after division
  if (difference > 180.0)
    difference -= 360.0;
  if (difference < -180.0)
    difference += 360.0;

  moveInDegree(difference);
  // DON'T FORGET to update currentAngle after the move!
  _currentAngle = targetAngle;
}

void CustomStepper::sweep(float startAngle, float endAngle, float speed)
{
  setSpeed(speed); // Set the speed once at the beginning

  // Move to the start position first
  moveToAngle(startAngle);

  // Sweep forever until a new command breaks it
  while (true)
  {
    // Move from startAngle to endAngle
    moveToAngle(endAngle);
    // Move from endAngle back to startAngle
    moveToAngle(startAngle);

    // Check if new serial data arrived to break the loop
    if (Serial.available() > 0)
    {
      break; // Exit the infinite sweep loop
    }
  }
}
