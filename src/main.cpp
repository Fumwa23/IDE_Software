#include <Arduino.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"

// LED Pins
#define LED1 21
#define LED2 34
#define LED3 38
#define LED4 39
#define LED5 40

// ADC Configuration
#define JOYSTICK_X_CHANNEL ADC1_CHANNEL_8 // GPIO9
#define JOYSTICK_Y_CHANNEL ADC1_CHANNEL_6 // GPIO7
#define DEFAULT_VREF 1100                 // Default reference voltage in mV
#define SAMPLES 32                        // Multisample count

// Filtering
float filtered_x = 0;
float filtered_y = 0;
const float alpha = 0.1; // Low-pass filter factor

// Calibration characteristics
esp_adc_cal_characteristics_t adc_chars;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Configure ADC1 width and attenuation
  adc1_config_width(ADC_WIDTH_BIT_13);
  adc1_config_channel_atten(JOYSTICK_X_CHANNEL, ADC_ATTEN_DB_12);
  adc1_config_channel_atten(JOYSTICK_Y_CHANNEL, ADC_ATTEN_DB_12);

  // Characterize ADC
  esp_adc_cal_value_t cal_type = esp_adc_cal_characterize(
      ADC_UNIT_1, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_13, DEFAULT_VREF, &adc_chars);

  // Print calibration method
  if (cal_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    Serial.println("ADC Calibration: Two Point");
  } else if (cal_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    Serial.println("ADC Calibration: eFuse Vref");
  } else {
    Serial.println("ADC Calibration: Default Vref");
  }

  // Initial filtered values
  filtered_x = esp_adc_cal_raw_to_voltage(adc1_get_raw(JOYSTICK_X_CHANNEL), &adc_chars);
  filtered_y = esp_adc_cal_raw_to_voltage(adc1_get_raw(JOYSTICK_Y_CHANNEL), &adc_chars);

  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, HIGH);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED2, HIGH);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED3, HIGH);
  pinMode(LED4, OUTPUT);
  digitalWrite(LED4, HIGH);
  pinMode(LED5, OUTPUT);
  digitalWrite(LED5, HIGH);
  Serial.println("LEDs initialized");
}

// Helper: Multisample and return calibrated voltage (in mV)
uint32_t read_averaged_voltage(adc1_channel_t channel) {
  uint32_t total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += adc1_get_raw(channel);
  }
  uint32_t avg = total / SAMPLES;
  return esp_adc_cal_raw_to_voltage(avg, &adc_chars);
}

void loop() {
  // Read joystick values with filtering
  uint32_t raw_x = read_averaged_voltage(JOYSTICK_X_CHANNEL);
  uint32_t raw_y = read_averaged_voltage(JOYSTICK_Y_CHANNEL);

  filtered_x = alpha * raw_x + (1 - alpha) * filtered_x;
  filtered_y = alpha * raw_y + (1 - alpha) * filtered_y;

  // Print results
  Serial.print("Joystick X: ");
  Serial.print(raw_x);
  Serial.print(" mV (Filtered: ");
  Serial.print((int)filtered_x);
  Serial.print("), Y: ");
  Serial.print(raw_y);
  Serial.print(" mV (Filtered: ");
  Serial.print((int)filtered_y);
  Serial.println(")");

  if (raw_x > 2500 && raw_y > 2500) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }
  else if (raw_x < 1500 && raw_y > 2500) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  } else if (raw_x > 2500 && raw_y < 1500) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
  } else if (raw_x < 1500 && raw_y < 1500) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
  } else {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }

  delay(100); // Tune as needed
}
