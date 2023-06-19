//
// interrupt service testing
// (to support debounced button presses and simple beeps on one timer)
//

/*
   Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to measure the time from the SW is pressed,
   debouncing time is 100ms => SW is considered pressed if timer count is > 5, then call / flag SW is pressed
   When the SW is released, timer will count (debounce) until more than 50ms until consider SW is released.
   We can set to flag or call a function whenever SW is pressed more than certain predetermined time, even before
   SW is released.
*/

// #define USE_SERIAL_DEBUG

#include "ButtonDebounceSetup.h"

void setup()
{
  pinMode( 4, OUTPUT);
  pinMode( 5, OUTPUT);

  pinMode( SWPin, INPUT_PULLUP);
  pinMode( SpkrPin, OUTPUT);

#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);
  while (!Serial);

  Serial.print(F("\nStarting SwitchDebounce on "));
  Serial.println(BOARD_TYPE);
  Serial.println(TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));
#endif

  // Timer0 is used for micros(), millis(), delay(), etc and can't be used
  // Select Timer 1-2 for UNO, 1-5 for MEGA, 1,3,4 for 16u4/32u4
  // Timer 2 is 8-bit timer, only for higher frequency
  // Timer 4 of 16u4 and 32u4 is 8/10-bit timer, only for higher frequency
  
#if USE_TIMER_1

  ITimer1.init();

  // Using ATmega328 used in UNO => 16MHz CPU clock ,

  if (ITimer1.attachInterruptInterval(INTERRUPT_INTERVAL_MS, TimerHandler))
  {
#ifdef USE_SERIAL_DEBUG
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
#endif
  }
  else {
#ifdef USE_SERIAL_DEBUG
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
#endif
  }
#elif USE_TIMER_3

  ITimer3.init();

  if (ITimer3.attachInterruptInterval(INTERRUPT_INTERVAL_MS, TimerHandler))
  {
#ifdef USE_SERIAL_DEBUG
    Serial.print(F("Starting  ITimer3 OK, millis() = ")); Serial.println(millis());
#endif
  }
  else {
#ifdef USE_SERIAL_DEBUG
    Serial.println(F("Can't set ITimer3. Select another freq. or timer"));
#endif
  }
#endif
}

void loop()
{
  if( my_press) {
    my_press = false;
    digitalWrite(4, HIGH);
    delay(100);
    digitalWrite(4, LOW);
    BeepCount = 100;
  }

}
