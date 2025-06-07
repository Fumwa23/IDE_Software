#include <Wire.h>
#include "PSPJoystick.h"
#include <Arduino.h>

// LED Pins
#define LED1 21
#define LED2 34
#define LED3 38
#define LED4 39
#define LED5 40

// Define pins for joystick
#define JOYSTICK_X_PIN 9
#define JOYSTICK_Y_PIN 7

PSPJoystick joystick;

// Low-pass filter variables
float filteredX = 0;
float filteredY = 0;
const float alpha = 0.5; // Adjust for desired smoothness

void setup() {
  Serial.begin(115200);

  pinMode(JOYSTICK_X_PIN, INPUT_PULLDOWN);
  pinMode(JOYSTICK_Y_PIN, INPUT_PULLDOWN);

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
  Serial.println("LEDs initialized");

  // Initialize filters with the first read value
  filteredX = joystick.getX();
  filteredY = joystick.getY();
}

void loop() {
  int xRaw = joystick.getX();
  int yRaw = joystick.getY();

  // Apply low-pass filter
  filteredX = alpha * xRaw + (1 - alpha) * filteredX;
  filteredY = alpha * yRaw + (1 - alpha) * filteredY;

  Serial.print("Joystick Raw X: ");
  Serial.print(xRaw);
  Serial.print(", Filtered X: ");
  Serial.print((int)filteredX);
  Serial.print(" | Raw Y: ");
  Serial.print(yRaw);
  Serial.print(", Filtered Y: ");
  Serial.println((int)filteredY);

  delay(100); // Adjust delay as needed
}
