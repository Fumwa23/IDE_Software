#ifndef PSPJOYSTICK_H
#define PSPJOYSTICK_H

#include "driver/adc.h"
#include "esp_adc_cal.h"

class PSPJoystick {
public:
    PSPJoystick();
    void setup(adc1_channel_t pinX, adc1_channel_t pinY, int samples = 32);

    // Manual calibration
    void calibrate(int centreXLow, int centreXHigh, int minX, int maxX, int centreYLow, int centreYHigh, int minY, int maxY);

    // Raw voltages
    int getX();
    int getY();

    // Calibrated offset (from centre)
    int getCalibratedX();
    int getCalibratedY();

    // Mapped to -100 to +100 range
    int getMappedX();
    int getMappedY();

private:
    adc1_channel_t _channelX;
    adc1_channel_t _channelY;
    int _samples;
    esp_adc_cal_characteristics_t _adc_chars;

    int _centreXLow, _centreXHigh, _minX, _maxX;
    int _centreYLow, _centreYHigh, _minY, _maxY;

    uint32_t readAveragedVoltage(adc1_channel_t channel);
    int mapWithClipping(int value, int in_min, int in_max, int out_min, int out_max);
};

#endif
