//
// Example code to control model car
// CarControl class handles most of the work, but the user can
// set the distance and other things
//
#include "CarControl.hh"

// create instance of car control class
car = CarControl = new CarControl();

car.setMode( CarControl::FixedDistance);   // set to fixed distance mode
car.setMode( CarControl::EnableButton);	   // enable the button to start
car.setMode( CarControl::BlinkingLights);  // blink the headlights while running

// NOTE:  change the value below to change the travel distance
car.setDistance( 10);			 // set to run for 10 revolutions

car.start();			         // start the car

// here the program will either start immediately, or wait for the user
// to press the button on the bottom of the car.  It will run for the
// specified distance and stop
