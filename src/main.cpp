#include <Arduino.h>
#include <Wire.h>

#include "ScrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"
#include "SaveManager.h"

// === Pin Definitions ===
#define SDA_PIN 35
#define SCL_PIN 33

#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN_BASE 6
#define DIR_PIN_BASE 8
#define ENABLE_PIN_BASE 4

#define STEP_PIN_ARM 2
#define DIR_PIN_ARM 5
#define ENABLE_PIN_ARM 1

#define JOYSTICK_X_CHANNEL ADC1_CHANNEL_8 // GPIO9
#define JOYSTICK_Y_CHANNEL ADC1_CHANNEL_6 // GPIO7

#define LED1 21
#define LED2 34
#define LED3 38
#define LED4 39
#define LED5 40

#define buttonPin 3

// === Globals ===
ScrollWheel scrollWheel;
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
StepperMotor armMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_ARM, DIR_PIN_ARM);
PSPJoystick joystick(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL, 2);  // Reduced samples for speed
SaveManager saveManager;

int previousPosition = 1;
unsigned long positionChangeTime = 0;
bool timerActive = false;

float targetPositionBase = 0;
float targetPositionArm = 0;

volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long lastInputPoll = 0;
const unsigned long inputPollInterval = 50;

int joystickX = 0;
int joystickY = 0;

// === Function Prototypes ===
void IRAM_ATTR handleButtonInterrupt();
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

  joystick.calibrate(
    1600, 2400, 750, 3200,  // X axis
    1400, 2250, 450, 2900   // Y axis
  );

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  lightLED(1);

  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, FALLING);
}

void loop() {
  unsigned long start = micros();
  unsigned long now = millis();

  // === Poll scroll wheel and joystick every 50ms ===
  if (now - lastInputPoll >= inputPollInterval) {
    scrollWheel.updatePosition();
    int currentPosition = scrollWheel.getPosition();
    if (currentPosition != previousPosition) {
      Serial.printf("Current Position: %d\n", currentPosition);
      lightLED(currentPosition);
      previousPosition = currentPosition;
      positionChangeTime = now;
      timerActive = true;
    }

    joystickX = joystick.getX();
    joystickY = joystick.getY();

    lastInputPoll = now;
  }

  // === Check scroll wheel timer ===
  if (timerActive && (now - positionChangeTime >= 1000)) {
    onTimerComplete(previousPosition);
    timerActive = false;
  }

  // === Check button interrupt flag ===
  if (buttonPressed && (now - lastDebounceTime >= debounceDelay)) {
    buttonPressed = false;
    runButtonPressed(previousPosition);
  }

  // === Update motors ===
  moveMotors(joystickX, joystickY);
  baseMotor.update();
  armMotor.update();

  Serial.println(micros() - start);
}

// === ISR ===
void IRAM_ATTR handleButtonInterrupt() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // Simple debounce check (avoid bouncing)
  if (interruptTime - lastInterruptTime > debounceDelay) {
    buttonPressed = true;
    lastDebounceTime = interruptTime;
  }

  lastInterruptTime = interruptTime;
}

// === Helper Functions ===
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
  // Serial.printf("Joystick X: %d, Y: %d\n", x, y);
  // Serial.printf("Base Motor Target Position: %ld\n", targetPositionBase);

  if (x > 2600) {
    targetPositionBase += 0.1;
    baseMotor.setTargetPosition(targetPositionBase);
  } else if (x < 1600) {
    targetPositionBase -= 0.1;
    baseMotor.setTargetPosition(targetPositionBase);
  }

  if (y > 2250) {
    targetPositionArm += 0.01;
    armMotor.setTargetPosition(targetPositionArm);
  } else if (y < 1400) {
    targetPositionArm -= 0.01;
    armMotor.setTargetPosition(targetPositionArm );
  }
}


















// #include <AccelStepper.h>
// // #include <Stepper.h>
// #include <StepperMotor.h>

// // Define stepper interface type and pins (adjust pins as needed)
// #define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
// #define STEP_PIN 6
// #define DIR_PIN 8

// #define BUTTON_PIN 3

// AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);
// // Stepper stepper2(200, STEP_PIN, DIR_PIN);

// StepperMotor stepperMotor(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Stepper Motor Test");

//   stepper.setMaxSpeed(4000);     // Set max speed (steps/sec)
//   stepper.setAcceleration(2000); // Set acceleration (steps/sec^2)

//   pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up resistor

//   // stepper2.setSpeed(1000); // Set speed for the Stepper library
// }

// void loop() {
//   unsigned long start = micros();
//   if (digitalRead(BUTTON_PIN) == LOW) {
//     //Serial.println("Button pressed! Starting movement...");
//     //stepper.moveTo(0);
//     stepperMotor.setTargetPosition(0);
//     //Serial.println("Moving to position 0");
//   } else {
//     //stepper.moveTo(1600);
//     stepperMotor.setTargetPosition(16000);
//    // Serial.println("Moving to position 1600");
//   }

//   // stepper.run();
//   stepperMotor.update();

//   Serial.println(micros() - start);
// }