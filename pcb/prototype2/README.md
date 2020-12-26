
The prototype board is to validate the proper use of the
[TCA8418](https://www.ti.com/lit/ds/symlink/tca8418.pdf) keyboard IC.

![Prototype Board](board.png)

There are a few errors in this first prototype board:

1. The through holes for the MCU were too small. Fortunately the MCU
   board could be squeezed in (just barely). The LCD holes were the
   correct diameter.
2. The LCD screen was too close to the top row of buttons. The goal
   was to have two rows of mechanical switches (Cherry MX), but
   fortunately there was (just barely) enough space for tactile
   switches.
3. Unused [TCA8418](https://www.ti.com/lit/ds/symlink/tca8418.pdf) key switches
   should have been tied to VCC (via resistors).
