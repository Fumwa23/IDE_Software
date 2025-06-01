#include <AccelStepper.h>
// #include <Stepper.h>

// Define stepper interface type and pins (adjust pins as needed)
#define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
#define STEP_PIN 3
#define DIR_PIN 5

AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);
// Stepper stepper2(200, STEP_PIN, DIR_PIN);

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper Motor Test");

  stepper.setMaxSpeed(4000);     // Set max speed (steps/sec)
  stepper.setAcceleration(2000); // Set acceleration (steps/sec^2)

  // stepper2.setSpeed(1000); // Set speed for the Stepper library
}

void loop() {
  // Move 100 steps forward
  // 1600 is full rotation
  stepper.moveTo(1600);
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

  // stepper2.step(4000);  // Move 100 steps forward
  // delay(500);           // Pause for 500ms
  // stepper2.step(-4000); // Move 100 steps backward
  // delay(2000);          // Pause for 500ms

  Serial.println("Cycle complete. Moving again...");
}