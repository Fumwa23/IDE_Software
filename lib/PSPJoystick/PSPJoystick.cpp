#include "PSPJoystick.h"
#include <Arduino.h>

PSPJoystick::PSPJoystick() : _channelX(ADC1_CHANNEL_0), _channelY(ADC1_CHANNEL_0), _samples(32) {}

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
