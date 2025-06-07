// #include <Wire.h>
// #include "scrollWheel.h"
// #include "StepperMotor.h"
// #include "PSPJoystick.h"

// #include <AccelStepper.h>

// // Define pins for scroll wheel I2C communication
// #define SDA_PIN 33
// #define SCL_PIN 35

// // Define pins for stpper motor control
// #define MOTOR_INTERFACE_TYPE AccelStepper::DRIVER
// #define STEP_PIN_BASE 6
// #define DIR_PIN_BASE 8
// #define ENABLE_PIN_BASE 4

// #define STEP_PIN_ARM 2
// #define DIR_PIN_ARM 5
// #define ENABLE_PIN_ARM 1

// // Define pins for joystick
// #define JOYSTICK_X_PIN 9
// #define JOYSTICK_Y_PIN 7

// // LED Pins
// #define LED1 21
// #define LED2 34
// #define LED3 38
// #define LED4 39
// #define LED5 40

// #define buttonPin 3


// ScrollWheel scrollWheel; // Starting position is 1
// StepperMotor baseMotor(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);
// PSPJoystick joystick;

// AccelStepper testStepper(MOTOR_INTERFACE_TYPE, STEP_PIN_BASE, DIR_PIN_BASE);

// // void setup() {
// //   Serial.begin(115200);
// //   Serial.println("Stepper Motor Test");

// //   stepper.setMaxSpeed(4000);     // Set max speed (steps/sec)
// //   stepper.setAcceleration(2000); // Set acceleration (steps/sec^2)

// //   // stepper2.setSpeed(1000); // Set speed for the Stepper library
// // }


// void setup() {
//   Serial.begin(115200);

//   // scrollWheel.setup(SDA_PIN, SCL_PIN);
//   // Serial.println("ScrollWheel Ready");

//   baseMotor.setTargetPosition(0);
//   pinMode(ENABLE_PIN_BASE, INPUT_PULLDOWN);
//   Serial.println("Stepper Motor Ready");

//   // joystick.setup(JOYSTICK_X_PIN, JOYSTICK_Y_PIN);
//   // Serial.println("PSP Joystick Ready");
//   // Serial.println("Setup complete. Starting main loop...");

//   pinMode(LED1, OUTPUT);
//   digitalWrite(LED1, HIGH);
//   pinMode(LED2, OUTPUT);
//   digitalWrite(LED2, HIGH);
//   pinMode(LED3, OUTPUT);
//   digitalWrite(LED3, HIGH);
//   pinMode(LED4, OUTPUT);
//   digitalWrite(LED4, HIGH);
//   pinMode(LED5, OUTPUT);
//   digitalWrite(LED5, HIGH);
//   Serial.println("LEDs initialized");

//   pinMode(buttonPin, INPUT_PULLUP);
//   Serial.println("Button pin set up with pull-up resistor");

//   testStepper.setMaxSpeed(4000);     // Set max speed (steps/sec)
//   testStepper.setAcceleration(2000); // Set acceleration (steps/sec^2)
// }

// void loop() {

//   Serial.println("Main loop running...");

//   // int xIncrement = joystick.getX();
//   // int yIncrement = joystick.getY();
//   // Serial.print("Joystick X: ");
//   // Serial.print(xIncrement);
//   // Serial.print(", Y: ");
//   // Serial.println(yIncrement);

//   // scrollWheel.updatePosition();
//   // Serial.print("Current Position: ");
//   // Serial.println(scrollWheel.getPosition());

//   digitalWrite(LED1, HIGH);

//   int buttonState = digitalRead(buttonPin);
//   // if (buttonState == LOW) {
//   //   Serial.println("Button pressed!");
//   //   baseMotor.setTargetPosition(0); // Reset position on button press
//   // } else {
//   //   baseMotor.setTargetPosition(1600); // Move to target position
//   // }

//   long baseTargetPosition = baseMotor.getTargetPosition();

//    // Move 100 steps forward
//   // 1600 is full rotation
//   testStepper.moveTo(1600);
//   while (testStepper.distanceToGo() != 0) {
//     testStepper.run();
//   }

//   delay(500); // Pause for 500ms

//   // Move 100 steps backward
//   testStepper.moveTo(0);
//   while (testStepper.distanceToGo() != 0) {
//     testStepper.run();
//   }

//   delay(500); // Pause for 500ms
  
//   delay(100); // Delay to make the output readable
// }
