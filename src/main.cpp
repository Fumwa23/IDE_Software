#include <Arduino.h>
#include <Wire.h>

#include "ScrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"

#include "SaveManager.h"

// Define pins for scroll wheel I2C communication
#define SDA_PIN 33
#define SCL_PIN 35
int previousPosition = 1; 

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
int previousButtonState = HIGH;

ScrollWheel scrollWheel; // Starting position is 1
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
PSPJoystick joystick(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL);

SaveManager saveManager;

void runButtonPressed();
void updateLEDs(int position);

void setup() {
  Serial.begin(115200);
  delay(1000);

  scrollWheel.setup(SDA_PIN, SCL_PIN);
  Serial.println("ScrollWheel Ready");

  baseMotor.setTargetPosition(0);
  pinMode(ENABLE_PIN_BASE, INPUT_PULLDOWN);
  Serial.println("Stepper Motor Ready");

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

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  // Setup button
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
  if (digitalRead(buttonPin) == LOW && previousButtonState == HIGH) {
    // Button was pressed
    Serial.println("Button pressed!");
    runButtonPressed();
  }
  previousButtonState = digitalRead(buttonPin);

  int x = joystick.getX();
  int y = joystick.getY();

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

  scrollWheel.updatePosition();
  int currentPosition = scrollWheel.getPosition();
  if (currentPosition != previousPosition) {
    Serial.printf("Current Position: %d\n", currentPosition);
    updateLEDs(currentPosition);

    Coordinate newCoordinate = saveManager.fetch(currentPosition);
    int newBasePosition = newCoordinate.x;
    int newArmPosition = newCoordinate.y; 
    baseMotor.setTargetPosition(newBasePosition);
    // armMotor.setTargetPosition(newArmPosition); // Assuming armMotor is defined similarly 

    previousPosition = currentPosition;
  }

  delay(100); // Adjust for responsiveness
}


//Function Handlers:

void runButtonPressed() {

  int savePosition = scrollWheel.getPosition();
  int basePosition = baseMotor.getCurrentPosition();
  int armPosition = 0;
  int coordinates[2] = {basePosition, armPosition};
  saveManager.save(savePosition, coordinates);
  Serial.printf("Saved position %d at base position %ld\n", savePosition, basePosition);

  digitalWrite(LED5, HIGH); // Indicate button press with LED5
  delay(500); // Debounce delay
  digitalWrite(LED5, LOW);
}


void updateLEDs(int position) {
  digitalWrite(LED1, position == 1 ? HIGH : LOW);
  digitalWrite(LED2, position == 2 ? HIGH : LOW);
  digitalWrite(LED3, position == 3 ? HIGH : LOW);
  digitalWrite(LED4, position == 4 ? HIGH : LOW);
  digitalWrite(LED5, position == 5 ? HIGH : LOW);
}