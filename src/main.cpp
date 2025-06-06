#include <Wire.h>
#include "scrollWheel.h"
#include "StepperMotor.h"
#include "PSPJoystick.h"

// Define pins for scroll wheel I2C communication
#define SDA_PIN 33
#define SCL_PIN 35

// Define pins for stpper motor control
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN_BASE 26
#define DIR_PIN_BASE 27
#define ENABLE_PIN_BASE 25

// Define pins for joystick
#define JOYSTICK_X_PIN 34
#define JOYSTICK_Y_PIN 32


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
