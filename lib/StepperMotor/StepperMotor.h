#pragma once
#include <AccelStepper.h>

class StepperMotor {
public:
    StepperMotor(uint8_t interfaceType, uint8_t stepPin, uint8_t dirPin);

    void setTargetPosition(long pos); // Set target position (0 to 3200)
    void update();
    long getCurrentPosition();
    long getTargetPosition() const;

private:
    AccelStepper stepper;
    long targetPosition = 0;
};