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

Software operation:

>
> power-up reads distance set from EEPROM
>    long (error) beep means invalid setting (default to 5)
> Idle mode
>    short press - drive for programmed distance
>    long press - move to 10's set mode
> 10's set mode
>    short press: add 10 revs
>    long press: to 1's set mode
> 1's set mode
>    short press: add 1 revs
>    long press: save setting and exit to IDLE
>

