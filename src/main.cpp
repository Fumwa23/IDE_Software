#include <Arduino.h>
#include <Wire.h>

#include "ScrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"

#include "SaveManager.h"

// Define pins for scroll wheel I2C communication
#define SDA_PIN 35
#define SCL_PIN 33
int previousPosition = 1;
unsigned long positionChangeTime = 0;
bool timerActive = false;

// Define pins for stpper motor control
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN_BASE 6
#define DIR_PIN_BASE 8
#define ENABLE_PIN_BASE 4
long targetPositionBase = 0;

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
StepperMotor armMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_ARM, DIR_PIN_ARM);
PSPJoystick joystick(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL);

SaveManager saveManager;

void runButtonPressed(int currentPosition);
void lightLED(int position);
void onTimerComplete(int currentPosition);

void moveMotors(int x, int y);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting setup...");

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
  lightLED(1); // Start with LED1 lit

  // Setup button
  pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
  scrollWheel.updatePosition();
  int currentPosition = scrollWheel.getPosition(); // (between 1 and 5)
  if (currentPosition != previousPosition) {
    Serial.printf("Current Position: %d\n", currentPosition);
    lightLED(currentPosition);
  
    previousPosition = currentPosition;
  
    // Reset timer
    positionChangeTime = millis();
    timerActive = true;
  }
  
  // Check if timer expired
  if (timerActive && (millis() - positionChangeTime >= 1000)) {
    onTimerComplete(currentPosition);
    timerActive = false;
  }

  int buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && previousButtonState == HIGH) {
    Serial.println("Button pressed!");
    runButtonPressed(currentPosition);
  }
  previousButtonState = buttonState;

  int x = joystick.getX();
  int y = joystick.getY();

  moveMotors(x, y);
}

// ============================================================================
// ============================== Helper Functions ============================
// ============================================================================


void runButtonPressed(int currentPosition) {
  int savePosition = scrollWheel.getPosition();
  int basePosition = baseMotor.getCurrentPosition();
  int armPosition = 0;
  int coordinates[2] = {basePosition, armPosition};
  saveManager.save(savePosition, coordinates);
  Serial.printf("Saved base position %ld at save position %d\n", basePosition, savePosition);
}

void lightLED(int position) {
  digitalWrite(LED1, position == 1 ? HIGH : LOW);
  digitalWrite(LED2, position == 2 ? HIGH : LOW);
  digitalWrite(LED3, position == 3 ? HIGH : LOW);
  digitalWrite(LED4, position == 4 ? HIGH : LOW);
  digitalWrite(LED5, position == 5 ? HIGH : LOW);
}

void onTimerComplete(int currentPosition) {
  Coordinate newCoordinate = saveManager.fetch(currentPosition);
  int newBasePosition = newCoordinate.x;
  int newArmPosition = newCoordinate.y; 
  baseMotor.setTargetPosition(newBasePosition);
  armMotor.setTargetPosition(newArmPosition);
  Serial.println("Timer completed, moving motors to saved position");
}

void moveMotors(int x, int y) {
  int baseTargetPosition = baseMotor.getTargetPosition();
  int armTargetPosition = armMotor.getTargetPosition();

  if (x > 2600) {
    targetPositionBase += 1;
    baseMotor.setTargetPosition(targetPositionBase);
  } else if (x < 1600) {
    targetPositionBase -= 1;
    baseMotor.setTargetPosition(targetPositionBase);
  }
  
  if (y > 2250) {
    armMotor.setTargetPosition(armTargetPosition + 100);
  }
  else if (y < 1400) {
    armMotor.setTargetPosition(armTargetPosition - 100);
  }


  // Serial.printf("X: %d mV, Y: %d mV\n", x, y);
  // Serial.printf("Base Target Position: %ld, Arm Target Position: %ld\n", 
  //               targetPositionBase, armMotor.getTargetPosition());

  baseMotor.update();
}