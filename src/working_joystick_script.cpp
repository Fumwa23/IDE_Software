// #include <Arduino.h>
// #include "PSPJoystick.h"

// // LED Pins
// #define LED1 21
// #define LED2 34
// #define LED3 38
// #define LED4 39
// #define LED5 40

// // Joystick ADC Channels
// #define JOYSTICK_X_CHANNEL ADC1_CHANNEL_8 // GPIO9
// #define JOYSTICK_Y_CHANNEL ADC1_CHANNEL_6 // GPIO7

// PSPJoystick joystick;

// void setup() {
//   Serial.begin(115200);
//   delay(1000);

//   // Initialise joystick
//   joystick.setup(JOYSTICK_X_CHANNEL, JOYSTICK_Y_CHANNEL);
//   // Manual calibration: centreLow, centreHigh, minOut, maxOut for X and Y in mV
//   joystick.calibrate(
//     1600, 2400, 750, 3200,  // X axis
//     1400, 2250, 450, 2900   // Y axis
//   );

//   // Setup LEDs
//   pinMode(LED1, OUTPUT);
//   pinMode(LED2, OUTPUT);
//   pinMode(LED3, OUTPUT);
//   pinMode(LED4, OUTPUT);
//   pinMode(LED5, OUTPUT);

//   digitalWrite(LED1, HIGH);
//   digitalWrite(LED2, HIGH);
//   digitalWrite(LED3, HIGH);
//   digitalWrite(LED4, HIGH);
//   digitalWrite(LED5, HIGH);

//   Serial.println("Joystick and LEDs initialised");
// }

// void loop() {
//   int x = joystick.getX();
//   int y = joystick.getY();

//   Serial.printf("Joystick X: %d mV, Y: %d mV\n", x, y);

//   if (x > 2600) {
//     digitalWrite(LED1, LOW);
//     digitalWrite(LED2, HIGH);
//   } else if (x < 1600) {
//     digitalWrite(LED1, HIGH);
//     digitalWrite(LED2, LOW);
//   } else {
//     digitalWrite(LED1, HIGH);
//     digitalWrite(LED2, HIGH);
//   }
  
//   if (y > 2250) {
//     digitalWrite(LED3, LOW);
//     digitalWrite(LED4, HIGH);
//   } else if (y < 1400) {
//     digitalWrite(LED3, HIGH);
//     digitalWrite(LED4, LOW);
//   } else {
//     digitalWrite(LED3, HIGH);
//     digitalWrite(LED4, HIGH);
//   }

//   delay(100); // Adjust for responsiveness
// }
