#ifndef PSPJOYSTICK_H
#define PSPJOYSTICK_H

#include "driver/adc.h"
#include "esp_adc_cal.h"

class PSPJoystick {
public:
    PSPJoystick();
    void setup(adc1_channel_t pinX, adc1_channel_t pinY, int samples = 32);
    int getX();
    int getY();

private:
    adc1_channel_t _channelX;
    adc1_channel_t _channelY;
    int _samples;
    esp_adc_cal_characteristics_t _adc_chars;

    uint32_t readAveragedVoltage(adc1_channel_t channel);
};

#endif
