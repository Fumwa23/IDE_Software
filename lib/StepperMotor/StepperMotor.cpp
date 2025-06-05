#include "StepperMotor.h"

StepperMotor::StepperMotor(uint8_t interfaceType, uint8_t stepPin, uint8_t dirPin)
    : stepper(interfaceType, stepPin, dirPin) {
    stepper.setMaxSpeed(8000);
    stepper.setAcceleration(2000);
    stepper.setCurrentPosition(0);
    targetPosition = 0;
}

void StepperMotor::setTargetPosition(long pos) {
    if (pos < 0) pos = 0;
    if (pos > 3200) pos = 3200;
    targetPosition = pos;
    stepper.moveTo(targetPosition);
}

void StepperMotor::update() {
    stepper.run();
}

long StepperMotor::getCurrentPosition() {
    return stepper.currentPosition();
}

long StepperMotor::getTargetPosition() const {
    return targetPosition;
}