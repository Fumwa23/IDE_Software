#include <AccelStepper.h>

// Define stepper interface type and pins (adjust pins as needed)
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN 2
#define DIR_PIN 3

AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Test");

  stepper.setMaxSpeed(500);     // Set max speed (steps/sec)
  stepper.setAcceleration(100); // Set acceleration (steps/sec^2)
}

void loop() {
  // Move 100 steps forward
  stepper.moveTo(100);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(500); // Pause for 500ms

  // Move 100 steps backward
  stepper.moveTo(0);
  while (stepper.distanceToGo() != 0) {
    stepper.run();
  }

  delay(500); // Pause for 500ms

  Serial.println("Cycle complete. Moving again...");
}