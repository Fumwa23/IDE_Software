#include <Wire.h>
#include "scrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"

// Define pins for scroll wheel I2C communication
#define SDA_PIN 33
#define SCL_PIN 35

// Define pins for stpper motor control
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN_BASE 8
#define DIR_PIN_BASE 6
#define ENABLE_PIN_BASE 4

#define STEP_PIN_ARM 2
#define DIR_PIN_ARM 5
#define ENABLE_PIN_ARM 1

// Define pins for joystick
#define JOYSTICK_X_PIN 9
#define JOYSTICK_Y_PIN 7

// LED Pins
#define LED1 21
#define LED2 34
#define LED3 38
#define LED4 39
#define LED5 40


ScrollWheel scrollWheel; // Starting position is 1
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
PSPJoystick joystick;

void setup() {
  Serial.begin(115200);
  scrollWheel.setup(SDA_PIN, SCL_PIN);
  Serial.println("ScrollWheel Ready");

  baseMotor.setTargetPosition(0);
  pinMode(ENABLE_PIN_BASE, OUTPUT);
  digitalWrite(ENABLE_PIN_BASE, LOW); 
  Serial.println("Stepper Motor Ready");

  joystick.setup(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
  Serial.println("PSP Joystick Ready");
  Serial.println("Setup complete. Starting main loop...");

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
}

void loop() {

  // Read joystick values
  int xIncrement = joystick.getX();
  int yIncrement = joystick.getY();
  Serial.print("Joystick X: ");
  Serial.print(xIncrement);
  Serial.print(", Y: ");
  Serial.println(yIncrement);


  scrollWheel.updatePosition();
  Serial.print("Current Position: ");
  Serial.println(scrollWheel.getPosition());

  long baseTargetPosition = baseMotor.getTargetPosition();
  baseMotor.setTargetPosition(baseTargetPosition + xIncrement);

  baseMotor.update();
  Serial.print("Stepper Motor Current Position: ");
  Serial.println(baseMotor.getCurrentPosition());
  Serial.print("Stepper Motor Target Position: ");
  Serial.println(baseMotor.getTargetPosition());
  
  delay(100); // Delay to make the output readable
}
