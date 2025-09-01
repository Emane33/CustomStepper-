// CustomStepper.h
#ifndef CustomStepper_h
#define CustomStepper_h

#include <Arduino.h>
#include <math.h>

class CustomStepper {
  public:
    // Constructor: Sets up the pins
    CustomStepper(int pin1, int pin2, int pin3, int pin4);

    // Core movement functions
    void moveToPosition(int targetPosition);
    void moveInDegree(float degree);
    void moveToAngle(float targetAngle);
    void sweep(float startAngle, float endAngle, float speed);

    // Configuration functions
    void setSpeed(float rpm);
    void enable();
    void disable();

  private:
    // Private variables
    int _pin1, _pin2, _pin3, _pin4;
    int _absolutePosition;
    int _sequenceIndex;
    long _delayBetweenSteps;
    float _currentAngle;
    const float _stepsPerDegree = 4096.0 / 360.0;
    
    const int _stepSequence[8][4] = {
      {1, 0, 0, 1}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0},
      {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}
    };

    // Private functions
    void performStep(int stepNumber);
};

#endif
