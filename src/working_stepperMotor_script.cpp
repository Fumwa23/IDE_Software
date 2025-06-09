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
//   if (digitalRead(BUTTON_PIN) == LOW) {
//     //Serial.println("Button pressed! Starting movement...");
//     //stepper.moveTo(0);
//     stepperMotor.setTargetPosition(0);
//     //Serial.println("Moving to position 0");
//   } else {
//     //stepper.moveTo(1600);
//     stepperMotor.setTargetPosition(1600);
//     Serial.println("Moving to position 1600");
//   }

//   // stepper.run();
//   stepperMotor.update();
// }