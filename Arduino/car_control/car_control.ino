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

#include <EEPROM.h>
#include "CarControl.hh"

CarControl car;

static int state = S_IDLE;	// initial state
static int speed;		// speed 0-255
// static int run_time;		// run time counter (temporary)
static int revs = 5;		// wheel revolutions to run

#define ACCEL_STEP 5

#define RUN_TICKS 200

void setup() {
  car.beep( BEEP_LONG);
  delay(1000);
  int lo = EEPROM.read(0);
  int hi = EEPROM.read(1);
  revs = (hi << 8) + lo;
  if( revs <= 0 || revs > 100) {
    car.beep(BEEP_ERROR);
    revs = 5;
  } else {
    car.beep( BEEP_SHORT);
  }

}


void loop() {

  // display state on the LEDs for debugging
  digitalWrite( LED1_PIN, state & 1);
  digitalWrite( LED2_PIN, (state & 2) != 0);
  digitalWrite( LED3_PIN, (state & 4) != 0);

  switch( state) {
  case S_IDLE:			// waiting for button press
    if( car.buttonPressed()) {	// short press, start running
      state = S_ACCEL;
      speed = 0;
    }
    if( car.buttonLong()) {
      car.beep(BEEP_HIGH);
      state = SP_TENS;
      revs = 0;
    }
    break;

  case S_ACCEL:			// accelerating to speed
    speed += ACCEL_STEP;
    if( speed >= 255) {
      speed = 255;
      state = S_RUN;
      car.clearRevs();
    }
    car.setSpeed( speed);
    break;

  case S_RUN:			// running - measure distance
    if( car.getRevs() >= revs) {
      state = S_DECEL;
    }
    break;

  case S_DECEL:			// decelerating to stop
    speed -= ACCEL_STEP;
    if( speed <= 0) {
      speed = 0;
      state = S_IDLE;
    }
    car.setSpeed( speed);
    break;

  case SP_TENS:			// program tens of rotations
    if( car.buttonPressed()) {
      if( revs >= 100) {
	car.beep(BEEP_ERROR);
	state = S_IDLE;
      } else {
	revs += 10;
	car.beep(BEEP_SHORT);
      }
    } else if( car.buttonLong()) {
      car.beep(BEEP_LOW);
      state = SP_ONES;
    }
    break;

  case SP_ONES:			// program ones of rotations
    if( car.buttonPressed()) {
      if( revs >= 100) {
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
    EEPROM.update( 0, (revs & 255));
    EEPROM.update( 1, (revs >> 8) & 255);
    state = S_IDLE;
    break;
    
  }

  car.updateRevs();
  car.updateButton();
  delay( 20);
}
