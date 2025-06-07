#include <Arduino.h>
#include <Wire.h>

#include "scrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"

// Define pins for scroll wheel I2C communication
#define SDA_PIN 33
#define SCL_PIN 35

// Define pins for stpper motor control
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN_BASE 6
#define DIR_PIN_BASE 8
#define ENABLE_PIN_BASE 4

#define STEP_PIN_ARM 2
#define DIR_PIN_ARM 5
#define ENABLE_PIN_ARM 1

// Joystick ADC Channels
#define JOYSTICK_X_CHANNEL ADC1_CHANNEL_8 // GPIO9
#define JOYSTICK_Y_CHANNEL ADC1_CHANNEL_6 // GPIO7

// LED Pins
#define LED1 21
#define LED2 34
#define LED3 38
#define LED4 39
#define LED5 40

#define buttonPin 3

ScrollWheel scrollWheel; // Starting position is 1
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
PSPJoystick joystick(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialise joystick
  joystick.calibrate(
    1600, 2400, 750, 3200,  // X axis - (deadzone: 1600-2400, min: 750, max: 3200)
    1400, 2250, 450, 2900   // Y axis
  );

  // Setup LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);

  Serial.println("Joystick and LEDs initialised");
}

void loop() {
  int x = joystick.getX();
  int y = joystick.getY();

  Serial.printf("Joystick X: %d mV, Y: %d mV\n", x, y);

  if (x > 2600) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
  } else if (x < 1600) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
  } else {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }
  
  if (y > 2250) {
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
  } else if (y < 1400) {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
  } else {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }

  delay(100); // Adjust for responsiveness
}
