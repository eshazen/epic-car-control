//
// CarControl class
//
// CarControl()                           constructor; setup I/O
// void setMode( options)                 set mode options (currently N/A)
// void clearMode( options)               reset mode options (currently N/A)
// void setSpeed( speed)                  set motor speed 0-255
// void setHeadLights( left, right)       set headlights on/off
// void setTailLights( left, right)       set taillights on/off
// void updateButton()                    call every 20ms to scan for button
// bool readButton()                      read currentbutton status
// bool buttonPressed()                   if button pressed since last call
// bool buttonLong()                      if last press was a long press
// void updateRevs()                      call every 20ms to check rev sensor
// void clearRevs()                       clear the rev sensor
// int getRevs()                          read rev sensor count
// int readSensor()                       return analog sensor reading
//                                        as 1024-reading

#ifndef CarControl_h
#define CarControl_h

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

// car states
enum { 
  // normal run states
  S_IDLE, S_ACCEL, S_RUN, S_DECEL,
  // programming states
  SP_TENS, SP_ONES, SP_EXIT, S_DIAG, S_DIAG1, S_DIAG2,
  S_CALIB, S_CALIB1, S_CALIB2
};

// button press states
enum {
  B_IDLE, B_PRESS, B_WAIT, B_RELEASE
};

// other constants
#define TICK_MS 20		// approximate refresh rate in ms

#define ACCEL 5		         // acceleration in speed units per tick
#define BUTTON_DEBOUNCE_TICKS 5  // button debounce in 20ms ticks
#define BUTTON_LONG_PRESS 100    // button long press in 20ms ticks

#define SENSOR_DELTA 50		 // proximity sensor threshold (change)
#define SENSOR_DEBOUNCE_TICKS 5	 // sensor debounce time in 20ms ticks

#define SENS_AVG_COUNT 25	// number of ticks to average for sensor test

// tone / duration pairs for beeper
struct {
  int pitch;
  int dur;
} tones[] = {
  {440, 100},			// short
  {440, 500},			// long
  {880, 20},			// tick
  {880, 500},			// high
  {220, 500},			// low
  {660, 1000}			// error
};

enum {
  BEEP_SHORT = 0,
  BEEP_LONG = 1,
  BEEP_TICK = 2,
  BEEP_HIGH = 3,
  BEEP_LOW = 4,
  BEEP_ERROR = 5
};

// these currently N/A
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
  void beep( int type);
  void updateButton();
  bool buttonPressed();
  bool buttonLong();
  int readButton();
  void updateRevs();
  void clearRevs();
  int getRevs();
  int readSensor();
  int s_min, s_max;		// sensor min and max from EEPROM/calibration
  
private:
  // variables
  int m_speed;  // current motor speed
  int distance;   // distance since start
  CarControlOptions options;
  // button stuff
  uint8_t debounceTime;		// debounce timer
  uint8_t buttonState;		// button state
  uint8_t pressTime;		// button press time for long press
  bool pressed;			// button pressed
  bool longpress;               // long press
  bool didpress;		// recorded a press
  // rev sensor stuff
  int revCount;			// revolution counter
  uint8_t revTime;		// revolution "debounce" time
  int sens, sens0;		// current and previous sensor readings
  int s_thr;			// sensor threshold
};

//
// constructor, setup hardware and pins
//
CarControl::CarControl() {
  
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
  // enable rotation sensor
  digitalWrite( SENS_LED_PIN, HIGH);
  
  debounceTime = distance = m_speed = 0;
  pressTime = 0;
  buttonState = B_IDLE;
  longpress = didpress = pressed = false;
  revCount = 0;
  revTime = 0;
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

void CarControl::beep( int type) {
  tone( SPKR_PIN, tones[type].pitch, tones[type].dur);
  // tone( SPKR_PIN, 440, 100);
}

//
// check for button press
// set pressed for short press and longpress for long press
// currently only triggers on release for short press
//

void CarControl::updateButton() {

  switch( buttonState) {

  // idle: check for initial press
  case B_IDLE:
    if( !digitalRead( BUTTON_PIN)) {
      debounceTime = BUTTON_DEBOUNCE_TICKS;
      buttonState = B_PRESS;
    }
    break;
    
  // pressed: wait for countdown 
  case B_PRESS:
    if( --debounceTime == 0) {	// count down complete?
      if( digitalRead( BUTTON_PIN)) {
	// released, set debounce count and go to released state
	debounceTime = BUTTON_DEBOUNCE_TICKS;
	buttonState = B_RELEASE;
	longpress = false;
	pressed = true;
	didpress = true;
      } else {
	// not released, go to wait state
	buttonState = B_WAIT;
	pressTime = BUTTON_DEBOUNCE_TICKS;
      }
    }
    break;

  // wait for release, check for long press
  case B_WAIT:
    // register a long press if not already done
    if( pressTime > BUTTON_LONG_PRESS && !didpress) {
      	longpress = true;
	pressed = false;
	didpress = true;	// protect against duplcate presses
    }
    
    if( digitalRead( BUTTON_PIN)) { // released?
      debounceTime = BUTTON_DEBOUNCE_TICKS;
      buttonState = B_RELEASE;
      if( !didpress) {
	pressed = true;
	longpress = false;
      }
    } else {
      ++pressTime;
    }

    break;

  // wait for timeout
  case B_RELEASE:
    if( debounceTime) {
      --debounceTime;
    } else {
      buttonState = B_IDLE;
      didpress = false;
    }
    break;
  }


}

//
// return true on short press
//
bool CarControl::buttonPressed() {
  if( pressed) {
    pressed = false;
    return true;
  }
  return false;
}

//
// return true on long press
//
bool CarControl::buttonLong() {
  if( longpress) {
    longpress = false;
    return true;
  }
  return false;
}


//
// read revolution sensor
// update revCount
//
void CarControl::updateRevs() {
  if( revTime) {
    --revTime;
  } else {
    sens0 = sens;
    sens = analogRead( SENS_INPUT_PIN);
    if( sens < s_thr && sens0 >= s_thr) {
      revTime = SENSOR_DEBOUNCE_TICKS;
      ++revCount;
      beep(BEEP_TICK);
    }
  }
}

//
// clear the revolution sensor
// also, recalculate threshold
//
void CarControl::clearRevs() {
  revCount = 0;
  s_thr = (s_min + s_max)/2;
}

//
// read the revolution sensor
//
int CarControl::getRevs() {
  return revCount;
}

//
// read button status now
//
int CarControl::readButton() {
  return digitalRead( BUTTON_PIN);
}


//
// read sensor as (1024-reading) with max 255
//
int CarControl::readSensor() {
  int v = 1023 - analogRead( SENS_INPUT_PIN);
  if( v < 0)
    v = 0;
  if( v > 255)
    v = 255;
  return v;
}

#endif
