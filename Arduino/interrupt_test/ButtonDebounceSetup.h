//
// setup stuff for button debounce
//

#define USE_TIMER_3     true

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "TimerInterrupt.h"

unsigned int SWPin = A1;
unsigned int SpkrPin = 16;

#define INTERRUPT_INTERVAL_MS 1
#define BUTTON_DOWN_TIME 20

volatile unsigned char InterruptCount = 0;
volatile unsigned char BeepCount = 0;
volatile bool my_press = false;
volatile unsigned char buttonTime = 0;

void TimerHandler() {

  if( buttonTime) {
    --buttonTime;
  } else {		/* button press timer not active */
    if ( (!digitalRead(SWPin)) ) {
      buttonTime = BUTTON_DOWN_TIME;
      my_press = true;
    }
  }
  
  // beep
  if( BeepCount) {
    BeepCount--;
    digitalWrite( SpkrPin, (BeepCount & 1));
  }


}

