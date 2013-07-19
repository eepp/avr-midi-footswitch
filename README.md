avr-midi-footswitch
===================

This is **avr-midi-footswitch**, a MIDI output footswitch module
controller using an Atmel AVR MCU (Arduino MCUs). The firmware weights
only 621 bytes!

A MIDI footswitch module is a module with switches you can press
with your foot. Each time you press a switch, a MIDI cc (control
change) message is sent to the MIDI output. It's mostly used by
musicians, especially guitarists and bass players, with software
effects and stomp boxes like Guitarix, AmpliTube and Guitar Rig.

This MIDI footswitch module also features a MIDI cc synchronization
button which can be used to resend the current MIDI cc values of
all footswitches.

The MIDI channel and cc numbers are all configurable at compile
time. You can also set the bypass cc value (0 or 127) depending
on your target software.

![Circuit diagram](https://raw.github.com/eepp/avr-midi-footswitch/master/circuit/avr-midi-footswitch.png)

A MIDI footswitch doesn't need lots of components: the MCU, a few
capacitors (for decoupling and to support the crystal oscillator),
a crystal oscillator (I used 8 MHz), pull-down resistors (10 kΩ)
and the actual footswitches. The MIDI output also needs a 220 Ω
resistor.

configuration
-------------

Before compiling the firmware, you need to configure a few things.

In the `Makefile`:

 * modify the `F_CPU` variable to match your MCU frequency (don't forget the `UL` suffix)
 * modify the `MCU` variable to match your exact AVR MCU model

You may also modify the `PROGFLAGS` if you intend to program your
chip with something else than USBasp (but why would you do this?).

In `config.h`:

 * configure the desired MIDI channel for cc messages (`MIDI_CHAN`)
 * configure the desired number of footswitches (`NB_FS`)
 * configure the desired cc value when bypassing (`BYPASS_SENDS_ZERO`)
 * fill the static `g_fs` array with each footswitch configuration
 * if you're going to include a sync button:
  * `#define SYNC_BTN_PRESENT` 
  * sync button DDR goes into `g_sync_ddr`
  * sync button pin goes into `g_sync_pin`
  * sync button pin bit mask goes into `g_sync_mask`

In `main.c`: you might need to change a few register/bit names in the
source to match your specific AVR MCU. Look at the `init_uart`,
`init_io`, `wait_uart_tx_ready` and `uart_send` functions.

compiling
---------

    $ make
    
programming
-----------

    # make program

using
-----

Power on the circuit, plug the MIDI out cable to your MIDI input
(sound card, cheap USB MIDI cable, etc.) and enjoy in your favorite
compatible software.

todo
----

An interesting feature found on some MIDI footswitch modules is one or
more inputs for expression pedals. An expression pedal is just a (usually 10 kΩ)
potentiometer plugged in using a stereo ¼'' jack. On the circuit, you
can see pin PC5 that's free on purpose: the associated ADC5 channel can
be used to read such an analog value and send it as a MIDI cc value (0..127).

Other ADC channels could be used by relocating some footswitches inputs
to e.g. port D.
