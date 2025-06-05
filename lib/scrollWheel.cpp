#include "scrollWheel.h"
#include <Wire.h>

void ScrollWheel::setup(int sdaPin, int sclPin) {
    Wire.begin(sdaPin, sclPin);
    if (!encoder.begin()) {
        while (1); // Halt if encoder is not detected
    }
    lastAngle = encoder.rawAngle();
}

void ScrollWheel::updatePosition() {
    int angle = encoder.rawAngle();
    if (angle == -1) return; // Error reading

    int diff = angle - lastAngle;
    if (diff > 1000) {
        if (position < 5) position++;
        lastAngle = angle;
    } else if (diff < -1000) {
        if (position > 1) position--;
        lastAngle = angle;
    }
}

int ScrollWheel::getPosition() const {
    return position;
}