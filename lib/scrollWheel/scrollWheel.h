#ifndef SCROLLWHEEL_H
#define SCROLLWHEEL_H

#include <AS5600.h>

class ScrollWheel {
public:
    void setup(int sdaPin, int sclPin);
    void updatePosition();
    int getPosition() const;

private:
    AS5600 encoder;
    int lastAngle = 0;
    int position = 1; // Range: 1-5
};

#endif