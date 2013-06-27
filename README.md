GFX-unlocker
============

Simple screen unlocker known from Android smartphones.

Written for ChibiOS/RT and ChibiOS/GFX. Provided example was tested with Embest
DM-STF4BB board and DM-LCD35RT LCD display but should work on any supported
hardware.

Unlocker (displayUnlockerSetup function) is intended to start before any other
real action happens. Demo uses thread-based delays (gfxSleepMilliseconds), so
make sure no unwanted thread can take control while unlocker waits.

There is a function (displayUnlockerSetup) that provides configuration
possibility. It displays the same unlocker which allows you to draw pattern that
will be stored and used in later unlockings.

The unlock pattern must be stored as one-dimensional array of uint8_t's, thus
matrix size must not exceed 15x16. Unlock pattern is a sequence of ring
numbers. Rings are numbered from 1, from left to right, starting with topmost
row. Below there is an example of each ring number for 3x3 matrix.

[1] [2] [3]

[4] [5] [6]

[7] [8] [9]

This is the order of unlock movements between each ring in demo setup. The
sequence form 1 to 7 shows what pattern to follow in order to unlock app
compiled without modifications.

(1) (5) (.)
 |   | \
(2) (6) (4)
   X   /
(7) (3) (.)

Hardcoded in main.c unlocking sequence is: 1, 4, 8, 6, 2, 5, 7, 0, 0.
