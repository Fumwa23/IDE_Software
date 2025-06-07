// #include <AS5600.h>
// #include <Wire.h>

// AS5600 encoder;

// #define SDA_PIN 35
// #define SCL_PIN 33

// void setup() {
//   Serial.begin(115200);

//   // Initialize I2C with custom SDA and SCL pins
//   Wire.begin(SDA_PIN, SCL_PIN);

//   if (!encoder.begin()) {
//     Serial.println("AS5600 not found");
//     while (1)
//       ; // Halt if encoder is not detected
//   }

//   Serial.println("AS5600 Encoder Test");
// }

// void loop() {
//   // Check if magnet is detected
//   if (encoder.detectMagnet()) {
//     int angle = encoder.rawAngle(); // Get the raw angle value

//     if (angle == -1) {
//       Serial.println("Error reading angle");
//     } else {
//       Serial.print("Raw Angle: ");
//       Serial.println(angle);
//     }
//   } else {
//     Serial.println("No magnet detected!");
//   }

//   delay(100); // Delay to make the output readable
// }
