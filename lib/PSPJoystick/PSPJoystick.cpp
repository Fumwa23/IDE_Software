#include "PSPJoystick.h"
#include <Arduino.h>

PSPJoystick::PSPJoystick()
    : _channelX(ADC1_CHANNEL_0), _channelY(ADC1_CHANNEL_0),
      _samples(32),
      _centreXLow(0), _centreXHigh(0), _minX(0), _maxX(3300),
      _centreYLow(0), _centreYHigh(0), _minY(0), _maxY(3300) {}

void PSPJoystick::setup(adc1_channel_t pinX, adc1_channel_t pinY, int samples) {
    _channelX = pinX;
    _channelY = pinY;
    _samples = samples;

    adc1_config_width(ADC_WIDTH_BIT_13);
    adc1_config_channel_atten(_channelX, ADC_ATTEN_DB_12);
    adc1_config_channel_atten(_channelY, ADC_ATTEN_DB_12);

    esp_adc_cal_characterize(
        ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_13, 1100, &_adc_chars);
}

uint32_t PSPJoystick::readAveragedVoltage(adc1_channel_t channel) {
    uint32_t total = 0;
    for (int i = 0; i < _samples; i++) {
        total += adc1_get_raw(channel);
    }
    uint32_t avg = total / _samples;
    return esp_adc_cal_raw_to_voltage(avg, &_adc_chars);
}

int PSPJoystick::getX() {
    return static_cast<int>(readAveragedVoltage(_channelX));
}

int PSPJoystick::getY() {
    return static_cast<int>(readAveragedVoltage(_channelY));
}

void PSPJoystick::calibrate(int centreXLow, int centreXHigh, int minX, int maxX, int centreYLow, int centreYHigh, int minY, int maxY) {
    _centreXLow = centreXLow;
    _centreXHigh = centreXHigh;
    _minX = minX;
    _maxX = maxX;

    _centreYLow = centreYLow;
    _centreYHigh = centreYHigh;
    _minY = minY;
    _maxY = maxY;
}

int PSPJoystick::mapWithClipping(int value, int in_min, int in_max, int out_min, int out_max) {
    if (value < in_min) value = in_min;
    if (value > in_max) value = in_max;
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int PSPJoystick::getMappedX() {
    int x = getX();
    if (x >= _centreXHigh)
        return mapWithClipping(x, _centreXHigh, _maxX, 0, 100);
    else if (x <= _centreXLow)
        return mapWithClipping(x, _minX, _centreXLow, -100, 0);
    else
        // If x is between centreXLow and centreXHigh, return 0
        return 0;
}

int PSPJoystick::getMappedY() {
    int y = getY();
    if (y >= _centreYHigh)
        return mapWithClipping(y, _centreYHigh, _maxY, 0, 100);
    else if (y <= _centreYLow)
        return mapWithClipping(y, _minY, _centreYLow, -100, 0);
    else
        // If y is between centreYLow and centreYHigh, return 0
        return 0; 
}
