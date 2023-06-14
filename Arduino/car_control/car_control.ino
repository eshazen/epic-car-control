
#include "CarControl.hh"

CarControl car;

void setup() {
	car.setMode( FixedDistance);
}


void loop() {
     car.setHeadLights( 1, 0);
     delay(100);
     car.setHeadLights( 0, 1);
     delay(100);
     car.setHeadLights( 0, 0);     
     delay(500);
     car.setTailLights( 1, 0);
     delay(100);
     car.setTailLights( 0, 1);
     delay(100);
     car.setTailLights( 0, 0);
     car.beep(500);
     delay(1000);
}