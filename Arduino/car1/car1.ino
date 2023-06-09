/*
 * EPIC model car
 * Initial hardware test-  blink LEDs, beep the beeper, run motor for 1s
 * rinse and repeat...
 */

// pins
#define LED1_PIN 4
#define LED2_PIN 5
#define LED3_PIN 6
#define LED4_PIN 7

#define MOTOR_PIN 3
#define SENS_LED_PIN 2
#define SENS_TRAN_PIN A0

#define SERVO1_PIN 9
#define SERVO2_PIN 10

#define SPKR_PIN 16

#define BUTTON_PIN A1

// the setup function runs once when you press reset or power the board
void setup() {
  // LEDS as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(SPKR_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite( MOTOR_PIN, LOW);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED1_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED1_PIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED2_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED2_PIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED3_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED3_PIN, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED4_PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED4_PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second

  // beeps
  for( int i=0; i<100; i++) {
    digitalWrite( SPKR_PIN, HIGH);
    delay(1);
    digitalWrite( SPKR_PIN, LOW);
    delay(1);
  }
  for( int i=0; i<50; i++) {
    digitalWrite( SPKR_PIN, HIGH);
    delay(2);
    digitalWrite( SPKR_PIN, LOW);
    delay(2);
  }
  digitalWrite( MOTOR_PIN, HIGH);
  delay( 1000);
  digitalWrite( MOTOR_PIN, LOW);
}
