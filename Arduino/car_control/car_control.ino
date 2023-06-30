//
// EPIC Car Control
//
// power-up reads distance set from EEPROM
//    long (error) beep means invalid setting (default to 5)
// Idle mode
//    short press - drive for programmed distance
//    long press - move to 10's set mode
// 10's set mode
//    short press: add 10 revs
//    long press: to 1's set mode
// 1's set mode
//    short press: add 1 revs
//    long press: save setting and exit to IDLE
//
// display value on LEDs in binary while setting
// if no short presses, just display and leave unchanged
// so 3 long presses just displays current value
//
// Mods:
// Rev 5 - used for June class
// Rev 6 - sensor threshold changed to 25 for a couple of cars
// Rev 7 - add sensor calibration code.

// #define SERIAL_DEBUG

// please update me! (max is 15 on 4 LEDs)
#define REVISION 7

// #define DEBUG_STATE_ON_LEDS

#include <EEPROM.h>
#include "CarControl.hh"

CarControl car;			// create instance of car control

// variables for the state machine which controls the car
// (these probably shoud be moved to the C++ class...)
static int state = S_IDLE;	// initial state
static int speed;		// speed 0-255
static int revs = 5;		// wheel revolutions to run

#define MAX_REV 100		// rev setting limit (default)

static bool rev_change;		// flag: changed revs

static int s_sum;		// sum for averaging

// programming blink rate in ticks (20ms tick, approximately)
#define PROG_FAST_BLINK 6
#define PROG_SLOW_BLINK 20
static uint8_t blinkt;

//
// display a binary value 0-15 on the LEDs
//
void binaryLights( int v) {
  digitalWrite( LED1_PIN, v & 1);
  digitalWrite( LED2_PIN, (v & 2) != 0);
  digitalWrite( LED3_PIN, (v & 4) != 0);
  digitalWrite( LED4_PIN, (v & 8) != 0);
}
  


//
// setup - most hardware setup already done in CarControl()
// read the EEPROM and set the rev counter
//
void setup() {
#ifdef SERIAL_DEBUG
  Serial.begin(115200);
#endif  
  car.beep( BEEP_LONG);			  // start with a long beep
  binaryLights( REVISION);		  // display version for 1 sec
  delay(1000);
  // chase around the LEDs to test them
  for( int i=0; i<4; i++) {
    car.setHeadLights( 0, 1);
    car.setTailLights( 0, 0);
    delay(100);  
    car.setHeadLights( 1, 0);
    car.setTailLights( 0, 0);
    delay(100);  
    car.setHeadLights( 0, 0);
    car.setTailLights( 0, 1);
    delay(100);  
    car.setHeadLights( 0, 0);
    car.setTailLights( 1, 0);
    delay(100);  
  }
  // read the revolution setting from EEPROM (non-volatile memory)
  int lo = EEPROM.read(0);
  int hi = EEPROM.read(1);
  car.s_min = EEPROM.read(2);
  car.s_max = EEPROM.read(3);
  revs = (hi << 8) + lo;
  if( revs <= 0 || revs > MAX_REV) {			// if invalid, beep, set to 5
    car.beep(BEEP_ERROR);
    revs = 5;
  } else {						// make a happy sound
    for( int i=100; i<=400; i*=2) {
      tone( SPKR_PIN, i, 100);
      delay(100);
    }
    car.beep( BEEP_SHORT);
  }
  // twitch the motor
  car.setSpeed( 200);
  delay(100);
  car.setSpeed( 0);
  // if button pressed on power-up, go to diagnostic mode
  if( !car.readButton()) {
    //    state = S_DIAG;
    state = S_CALIB;    
    while( !car.readButton())				// wait for button release
      ;
    delay(100);
  } else {
    // start with taillights only on
    car.setHeadLights( 0, 0);
    car.setTailLights( 1, 1);
    
  }
}


//
// main loop - update the (Mealy) state machine every 20ms
//
void loop() {

#ifdef DEBUG_STATE_ON_LEDS
  // display FSM state on the LEDs for debugging
  binaryLights( state);
#endif  

  switch( state) {

  case S_IDLE:			// waiting for button press
    if( car.buttonPressed()) {	// short press, start running
      car.clearRevs();
      state = S_ACCEL;
      speed = 0;
    }
    if( car.buttonLong()) {	// long press, start programming
      car.beep(BEEP_HIGH);
      state = SP_TENS;
      blinkt = 0;
      rev_change = false;
    }
    break;

  case S_ACCEL:			// accelerating to speed
    car.setHeadLights( 0, 0);	// headlights off
    car.setTailLights( 0, 0);	// taillights off
    speed += ACCEL;
//    if( car.getRevs() >= revs) {
//      state = S_DECEL;		// hit the rev count - go decelerate
//    }
    if( speed >= 255) {		// max speed?
      speed = 255;
      state = S_RUN;		// go to running state
      car.clearRevs();
    }
    car.setSpeed( speed);
    break;

  case S_RUN:			// running - measure distance
    car.setHeadLights( 1, 1);	// headlights on
    car.setTailLights( 0, 0);	// taillights off
    if( car.getRevs() >= revs) {
      state = S_DECEL;		// hit the rev count - go decelerate
    }
    break;

  case S_DECEL:			// decelerating to stop
    car.setHeadLights( 0, 0);	// headlights off
    car.setTailLights( 1, 1);	// taillights on
    speed -= ACCEL;
    if( speed <= 0) {
      speed = 0;
      state = S_IDLE;		// stopped - go back to IDLE
    }
    car.setSpeed( speed);
    break;

  case SP_TENS:			// program tens of rotations
    // display tens on lights, slow blink
    if( blinkt == PROG_SLOW_BLINK)
      binaryLights( revs / 10);
    else if( blinkt == PROG_SLOW_BLINK*2) {
      blinkt = 0;
      binaryLights( 0);
    }
    if( car.buttonPressed()) {	// short press - increment count
      if( !rev_change) {
	revs = 0;		// changing the revs, zero first
	rev_change = true;
      }
      if( revs >= MAX_REV) {	// more than max?
	car.beep(BEEP_ERROR);	// yes, error beep
	state = S_IDLE;		// leave at error value
      } else {
	revs += 10;		// increment by 10
	car.beep(BEEP_SHORT);
      }
    } else if( car.buttonLong()) { // long press - go set ones
      car.beep(BEEP_LOW);
      state = SP_ONES;
      blinkt = 0;
    }
    break;

  case SP_ONES:			// program ones of rotations
    // display ones on lights, fast blink
    if( blinkt == PROG_FAST_BLINK)
      binaryLights( revs % 10);
    else if( blinkt == PROG_FAST_BLINK*2) {
      blinkt = 0;
      binaryLights( 0);
    }
    if( car.buttonPressed()) {
      if( !rev_change) {
	revs = 0;		// changing the revs, zero first
	rev_change = true;
      }
      if( revs >= MAX_REV) {
	car.beep(BEEP_ERROR);
	state = S_IDLE;
      } else {
	revs ++;
	car.beep(BEEP_SHORT);
      }
    } else if( car.buttonLong()) {
      car.beep(BEEP_LONG);
      state = SP_EXIT;
    }
    break;

  case SP_EXIT:			// program write data exit
    if( rev_change) {
      EEPROM.update( 0, (revs & 255));
      EEPROM.update( 1, (revs >> 8) & 255);
    }
    // start with taillights only on
    car.setHeadLights( 0, 0);
    car.setTailLights( 1, 1);
    state = S_IDLE;
    break;
    
  case S_DIAG:			// diagnostic display
    if( car.buttonPressed()) {
      state = S_DIAG1;
      // stop everything and get a sensor reading
      s_sum = 0;
      for( uint8_t i=0; i<SENS_AVG_COUNT; i++)
	s_sum += car.readSensor();
      s_sum /= SENS_AVG_COUNT;
      car.beep(BEEP_LONG);
    }
    break;

  case S_DIAG1:
    binaryLights( s_sum);
    if( car.buttonPressed()) {
      state = S_DIAG2;
      car.beep(BEEP_TICK);
    }
    break;

  case S_DIAG2:
    binaryLights( (s_sum >> 4));
    if( car.buttonPressed()) {
      state = S_DIAG;
      car.beep(BEEP_TICK);
    }      
    break;

  case S_CALIB:
    Serial.println("calib");
    binaryLights( 15);
    car.s_min = 1023;
    car.s_max = 0;
    state = S_CALIB1;
    break;

  case S_CALIB1:
    s_sum = car.readSensor();
    if( s_sum > car.s_max)  {
      car.s_max = s_sum;
      car.beep( BEEP_TICK);
    }
    if( s_sum < car.s_min) {
      car.s_min = s_sum;
      car.beep( BEEP_TOCK);
    }
    if( car.buttonPressed()) {
      state = S_CALIB2;
      car.beep(BEEP_LONG);
    }
    break;

  case S_CALIB2:
#ifdef SERIAL_DEBUG
    Serial.print("cal: ");
    Serial.print(car.s_min);
    Serial.print("-");
    Serial.println(car.s_max);
#endif    
    EEPROM.update(2, car.s_min);
    EEPROM.update(3, car.s_max);
    car.beep(BEEP_SHORT);
    // start with taillights only on
    car.setHeadLights( 0, 0);
    car.setTailLights( 1, 1);
    state = S_IDLE;
    break;
  }

  // do this every 20ms or so
  car.updateRevs();		// update revolution sensor
  car.updateButton();		// update button press/debounce
  delay( TICK_MS);		// delay 20ms
  ++blinkt;			// increment counter for blinking LEDs
}
