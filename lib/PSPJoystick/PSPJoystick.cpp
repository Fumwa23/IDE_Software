#include "PSPJoystick.h"
#include <Arduino.h>

PSPJoystick::PSPJoystick() : _pinX(-1), _pinY(-1) {}

void PSPJoystick::setup(int pinX, int pinY, int maxIncrement) {
    _pinX = pinX;
    _pinY = pinY;
    _maxIncrement = maxIncrement;
    pinMode(_pinX, INPUT);
    pinMode(_pinY, INPUT);
}

int PSPJoystick::getX() {
    int value = analogRead(_pinX);
    return value; // map(value, 0, 4095, -_maxIncrement, _maxIncrement);
}

int PSPJoystick::getY() {
    int value = analogRead(_pinY);
    return value; // map(value, 0, 4095, -_maxIncrement, _maxIncrement);
}