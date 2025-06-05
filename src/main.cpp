#include <Wire.h>
#include "scrollWheel.h"
#include "StepperMotor.h"

#define SDA_PIN 33
#define SCL_PIN 35
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN 26
#define DIR_PIN 27
#define ENABLE_PIN 25


ScrollWheel scrollWheel; // Starting position is 1
StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(115200);
  scrollWheel.setup(SDA_PIN, SCL_PIN);
  Serial.println("ScrollWheel Ready");

  baseMotor.setTargetPosition(0);
  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); 
  Serial.println("Stepper Motor Ready");
}

void loop() {

  scrollWheel.updatePosition();
  Serial.print("Current Position: ");
  Serial.println(scrollWheel.getPosition());

  baseMotor.update();
  Serial.print("Stepper Motor Current Position: ");
  Serial.println(baseMotor.getCurrentPosition());
  Serial.print("Stepper Motor Target Position: ");
  Serial.println(baseMotor.getTargetPosition());
  
  delay(100); // Delay to make the output readable
}
