#include <Wire.h>
#include "SCROLLWHEEL_H"

#define SDA_PIN 33
#define SCL_PIN 35

ScrollWheel scrollWheel;

void setup() {
    Serial.begin(115200);
    scrollWheel.setup(SDA_PIN, SCL_PIN);
    Serial.println("ScrollWheel Ready");
}

void loop() {
    scrollWheel.updatePosition();
    Serial.print("Current Position: ");
    Serial.println(scrollWheel.getPosition());

    delay(100); // Delay to make the output readable
}
