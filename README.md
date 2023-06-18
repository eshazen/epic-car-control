# epic-car-control

Arduino-based model car control for EPIC.  This is a PCB which mounts
on a model car used in the Partner Classes taught at EPIC.

* [Software Development Log](SoftwareLog.md)

## Hardware features

* Arduino pro micro socket
* MOSFET for motor control
* Optical sensor for wheel revolution counter
* Headlights and taillights with individual control
* Small speaker
* Bottom-mounted pushbutton switch for user inputs
* Screw terminal connectors for a 6V battery pack (4xAA) and motor
* Header connector for a power switch

## Software

Here's a tentative spec for the software:

### Normal mode (power-up without button pressed)

* One long beep
* Flash a chase on the 4 LEDs
* Twitch the motor
* Test the optical sensor, multiple beeps on fail

On button press:

* One short beep
* Accelerate to full speed
* Count wheel rotations to programmed value
* Decelerate to stop
* One short beep

### Programming mode 

Power-up with button pressed, or one long (2s) button press.

Resets to default of zero revolutions of the sensor.

* Two short beeps entering mode
* Each short button press adds 10 revs to the count
  <br>Count displayed in binary on the LEDs, maximum 9.
  <br>Short beep when count increments, long beep if at limit.
* Long press to advance to single revs mode (3 beeps entering mode)
* Each short button press adds 1 rev to the count
  <br>Count displayed in binary on the LEDs, maximum 9.
  <br>Short beep when count increments, long beep if at limit.
* Long press to exit programming mode
  <br>Three beeps, return to normal mode
  
