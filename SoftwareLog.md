# Software log

**2023-06-30** Working on sensor calibration

Created version 7.  Trying to get a sensor calibration code running.
Having trouble triggering it from the diagnostic mode.

**2023-06-29** First class

Deployed version 5.  All worked well on about half of the cars, but
the sensors didn't work well on the others.

**2023-06-19** Continuing on software.

The ISR stuff seems like a lot of work.  The debounce demo seems to
work fine with 20ms interrupts, so a main loop running at that speed
via delay() should be OK.  Here's the algo from the demo:

> Switch Debouncing uses high frequency hardware timer 50Hz == 20ms) to
> measure the time from the SW is pressed, debouncing time is 100ms =>
> SW is considered pressed if timer count is > 5, then call / flag SW is
> pressed When the SW is released, timer will count (debounce) until
> more than 50ms until consider SW is released.  We can set to flag or
> call a function whenever SW is pressed more than certain predetermined
> time, even before SW is released.

OK, first version is working ok.  Long/short button presses and rev
counter implemented using update() functions expected to be called
every ~20ms from loop().

**2023-06-18** Starting on working software.

Installed Arduino 2.1.0 as Appimage from the web.

`tone()` function works for beeping, and runs in background.

Idea:  use timer interrupts to record time/length of last button press.

Installing
[TimerInterrupt](https://github.com/khoih-prog/TimerInterrupt) v1.8.0
by Khoi Hoang, which claims to support the Leonardo.

This works OK but fights with tone(); Trying to create a unified ISR
which handles both, but the one with the button debounce demo seems to
take more than 1ms to run.

