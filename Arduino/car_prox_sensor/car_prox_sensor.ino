//
// test the proximity sensor
//

// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 4; // Analog output pin that the LED is attached to
const int sensorLEDPin = 2; // proximity sensor LED pin
const int frontLEDPin = 4;  // headlight pin

void setup() {
  Serial.begin(115200);
  pinMode( sensorLEDPin, OUTPUT);
  pinMode( frontLEDPin, OUTPUT);
  digitalWrite( sensorLEDPin, HIGH);  // turn on the LED
}

void loop() {
  Serial.println( analogRead( analogInPin));
  delay(500);
}
