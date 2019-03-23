AVR MIDI footswitch
===================
This is _AVR MIDI footswitch_, a configurable MIDI output footswitch
module which uses an AVR MCU (Arduino MCUs). The firmware weights only
about 700 bytes (ATmega88PA, four switches)!

A MIDI footswitch module is a module with switches you can press with
your foot. Each time you press a switch, the module sends a MIDI CC
(control change) to the MIDI output. It's mostly used by musicians,
especially guitarists and bass players, with software effects and stomp
boxes like Guitarix, AmpliTube, and Guitar Rig.

The MIDI footswitch module is fully configurable (see `config.h` and
`config.c`):

* Switch debounce time
* Number of footswitches
* UART configuration to adapt to your MCU
* Optional LED animation at power-up
* Per footswitch:
  * Whether the switch is a momentary or an on-off switch
  * Switch's data direction register, pin register, and pin position to
    use
  * MIDI channel, CC number, and CC values for on/off states to send
  * Whether this footswitch has an associated LED or not
  * LED's data direction register, port register, and pin position to
    use

Circuit example:

![Circuit diagram](https://raw.github.com/eepp/avr-midi-footswitch/master/circuit/avr-midi-footswitch.png)

A MIDI footswitch module doesn't need a lot of components: the MCU, a
few capacitors (for decoupling and to support the crystal oscillator), a
crystal oscillator (I used 8 MHz), pull-down resistors (10 kΩ) and the
actual switches. The MIDI output also needs a 220 Ω resistor and a
socket. If you want LEDs to indicate each footswitch's current state,
you also need a few more resistors to limit the current.

Configure
---------
Before you compile the firmware, you need to configure a few things:

In `Makefile`:

* Set the `F_CPU` variable to match your MCU's frequency (don't forget the `UL` suffix).
* Modify the `MCU` variable to match your exact AVR MCU model.

You can also modify the `PROGFLAGS` if you intend to program your chip
with something else than USBasp (but why would you do this?).

Edit `config.h` and `config.c` to configure the firmware. The `g_cfg`
array's size in `config.c` must match the value of `CFG_NB_FS`.

Compile
-------

    make

Program
-------

    make program

Use
---
Plug the MIDI cable to your MIDI input (sound card, cheap USB MIDI
cable, etc.), power on the module, and enjoy the device with your
favorite compatible software.
