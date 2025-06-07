#include "scrollWheel.h"
#include <Wire.h>

void ScrollWheel::setup(int sdaPin, int sclPin) {
    Wire.begin(sdaPin, sclPin);
    if (!encoder.begin()) {
        Serial.println("AS5600 not found");
        while (1); // Halt if encoder is not detected
    }
    lastAngle = encoder.rawAngle();
}

void ScrollWheel::updatePosition() {
    if (!encoder.detectMagnet()) {
        //Serial.println("No magnet detected!");
        return;
    }

    int angle = encoder.rawAngle();
    if (angle == -1) return; // Error reading

    int diff = angle - lastAngle;
    if (diff > 200) {
        position++;
        if (position > 5) position = 1;
        lastAngle = angle;
    } else if (diff < -200) {
        position--;
        if (position < 1) position = 5;
        lastAngle = angle;
    }
}

int ScrollWheel::getPosition() const {
    return position;
}