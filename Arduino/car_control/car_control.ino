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

// please update me!
#define REVISION 4

// #define DEBUG_STATE_ON_LEDS

#include <EEPROM.h>
#include "CarControl.hh"

CarControl car;			// create instance of car control

// variables for the state machine which controls the car
// (these probably shoud be moved to the C++ class...)
static int state = S_IDLE;	// initial state
static int speed;		// speed 0-255
static int revs = 5;		// wheel revolutions to run

#define MAX_REV 100		// rev setting limit

static bool rev_change;		// flag: changed revs

static int diag_page;
static int s_sum;

// programming blink rate in ticks
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
  car.beep( BEEP_LONG);
  // display version for 1 sec
  binaryLights( REVISION);
  delay(1000);
  // chase around the LEDs
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
  int lo = EEPROM.read(0);
  int hi = EEPROM.read(1);
  revs = (hi << 8) + lo;
  if( revs <= 0 || revs > MAX_REV) {
    car.beep(BEEP_ERROR);
    revs = 5;
  } else {
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
  if( !car.readButton()) {
    state = S_DIAG;
    diag_page = 0;
    while( !car.readButton())
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
    if( car.getRevs() >= revs) {
      state = S_DECEL;		// hit the rev count - go decelerate
    }
    if( speed >= 255) {		// max speed?
      speed = 255;
      state = S_RUN;		// go to running state
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
    binaryLights( diag_page);
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
    
  }

  // do this every 20ms or so
  car.updateRevs();		// update revolution sensor
  car.updateButton();		// update button press/debounce
  delay( 20);			// delay 20ms
  ++blinkt;
}
