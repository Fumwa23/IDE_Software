#ifndef PSPJOYSTICK_H
#define PSPJOYSTICK_H

class PSPJoystick {
public:
    PSPJoystick();  // Constructor
    void setup(int pinX, int pinY, int maxIncrement = 200);
    int getX();
    int getY();

private:
    int _pinX;
    int _pinY;
    int _maxIncrement = 200; // Default max increment
};

#endif
