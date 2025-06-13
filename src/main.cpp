#include <Arduino.h>
#include <Wire.h>

#include "ScrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"
#include "SaveManager.h"

#include "driver/rtc_io.h"

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

#define POWER_SWITCH_PIN 12

// === Globals ===
ScrollWheel scrollWheel;
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
StepperMotor armMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_ARM, DIR_PIN_ARM);
PSPJoystick joystick(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL, 2);  // Reduced samples for speed
SaveManager saveManager;

volatile bool goToSleep = true;

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

// === Function Declarations ===
void IRAM_ATTR handleButtonInterrupt();
void IRAM_ATTR powerOffInterrupt();
void runButtonPressed(int currentPosition);
void lightLED(int position);
void onTimerComplete(int currentPosition);
void moveMotors(int x, int y);

// === Main Program ===

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting setup...");

  // Deep Sleep Configuration
  gpio_hold_dis(GPIO_NUM_4);
  gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_4, 0);

  gpio_hold_dis(GPIO_NUM_1);
  gpio_set_direction(GPIO_NUM_1, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_1, 0);

  rtc_gpio_init(GPIO_NUM_12);
  rtc_gpio_set_direction(GPIO_NUM_12, RTC_GPIO_MODE_INPUT_ONLY);
  rtc_gpio_pullup_en(GPIO_NUM_12);
  rtc_gpio_hold_en(GPIO_NUM_12);

  pinMode(POWER_SWITCH_PIN, INPUT_PULLUP);
  goToSleep = digitalRead(POWER_SWITCH_PIN) == HIGH;

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, 0);
  attachInterrupt(digitalPinToInterrupt(POWER_SWITCH_PIN), powerOffInterrupt, RISING);

  // Scrollwheel
  scrollWheel.setup(SDA_PIN, SCL_PIN);
  Serial.println("ScrollWheel Ready");

  // Stepper motors
  baseMotor.setTargetPosition(0);
  armMotor.setTargetPosition(0);
  Serial.println("Stepper Motor Ready");

  // Joystick
  joystick.calibrate(
    1600, 2400, 750, 3200,  // X axis
    1400, 2250, 450, 2900   // Y axis
  );

  // LEDs
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  lightLED(1);

  // Button
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, FALLING);
}

void loop() {
  Serial.println("Looping...");
  
  if (goToSleep) {
    Serial.println("Entering deep sleep...");

    gpio_set_level(GPIO_NUM_4, 1); // Base enable pin
    gpio_hold_en(GPIO_NUM_4);

    gpio_set_level(GPIO_NUM_1, 1); // Arm enable pin
    gpio_hold_en(GPIO_NUM_1);

    delay(100);
    esp_deep_sleep_start();
  }

  unsigned long start = micros(); // measures loop lengh.
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

  // Serial.println(micros() - start);
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

void IRAM_ATTR powerOffInterrupt() {
  goToSleep = true;
  Serial.println("Power Off Interrupt Triggered");
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
analogWrite(LED1, position == 1 ? 30 : 0);
analogWrite(LED3, position == 3 ? 30 : 0);
analogWrite(LED2, position == 2 ? 30 : 0);
analogWrite(LED4, position == 4 ? 30 : 0);
analogWrite(LED5, position == 5 ? 30 : 0);
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
  if (x > 2600) {
    targetPositionBase += 0.04;
    baseMotor.setTargetPosition(targetPositionBase);
  } else if (x < 1600) {
    targetPositionBase -= 0.04;
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