//
// CarControl class
//
// CarControl()                      constructor; setup I/O
// setMode( options)                 set mode options
// clearMode( options)               reset mode options
// setSpeed( speed)                  set motor speed 0-255
// setHeadLights( left, right)       set headlights on/off
// setTailLights( left, right)       set taillights on/off
// readSensor

#ifndef CarControl_h
#define CarControl_h

#include <ezBuzzer.h>

// pins
#define LED1_PIN 4
#define LED2_PIN 5
#define LED3_PIN 6
#define LED4_PIN 7
#define MOTOR_PIN 3
#define SENS_LED_PIN 2
#define SENS_INPUT_PIN A0
#define SERVO1_PIN 9
#define SERVO2_PIN 10
#define SPKR_PIN 16
#define BUTTON_PIN A1

// states
#define S_IDLE 0
#define S_ACCEL 1
#define S_RUN 2
#define S_DECEL 3

// other constants
#define TARGET_DISTANCE 10   // desired distance in revolutions
#define ACCEL 5   // acceleration in speed units per tick
#define SENSOR_THRESHOLD 900  // proximity sensor threshold


enum CarControlOptions {
  FixedDistance, EnableButton, BlinkHeadlights, BlinkTaillights
};

class CarControl {
public:
  CarControl();
  void setMode( enum CarControlOptions); 
  void setSpeed( int speed);
  void setHeadLights( int left, int right);
  void setTailLights( int left, int right);
  void beep( int dur);
	       
private:
  // variables
  uint8_t state;  // current state from above
  int m_speed;  // current motor speed
  int distance;   // distance since start
  ezBuzzer* buzz;
  CarControlOptions options;
};

CarControl::CarControl() {
  
// initialize buzzer library
  buzz = new ezBuzzer( SPKR_PIN);
  // Declare output pins
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(SPKR_PIN, OUTPUT);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(SENS_LED_PIN, OUTPUT);

  // button is an input with pullup
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // start with motor and LEDs off
  digitalWrite( MOTOR_PIN, LOW);
  digitalWrite( LED1_PIN, LOW);
  digitalWrite( LED2_PIN, LOW);
  digitalWrite( LED3_PIN, LOW);
  digitalWrite( LED4_PIN, LOW);
  digitalWrite( SENS_LED_PIN, LOW);
  
  state = S_IDLE;

  buzz->beep(500);

}

void CarControl::setMode( enum CarControlOptions opt) {
  options = opt;
}



void CarControl::setSpeed( int speed) {
  m_speed = speed;
  analogWrite( MOTOR_PIN, m_speed);
}


void CarControl::setHeadLights( int left, int right) {
  digitalWrite( LED2_PIN, left);
  digitalWrite( LED1_PIN, right);
}

void CarControl::setTailLights( int left, int right) {
  digitalWrite( LED3_PIN, left);
  digitalWrite( LED4_PIN, right);
}

void CarControl::beep( int dur) {
  buzz->beep( dur);
}

#endif
