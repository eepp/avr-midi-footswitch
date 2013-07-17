#ifndef _CONFIG_H
#define _CONFIG_H

/* MIDI channel (0 means channel 1) */
#define MIDI_CHAN		0

/* switches debounce time (ms) */
#define SW_DEB_TIME		10.0

/* number of footswitches */
#define NB_FS			4

/* footswitches data direction registers */
static const volatile uint8_t* g_fs_ddrs[] = {
	&DDRC,
	&DDRC,
	&DDRC,
	&DDRC,
};

/* footswitches pins */
static const volatile uint8_t* g_fs_pins[] = {
	&PINC,
	&PINC,
	&PINC,
	&PINC,
};

/* footswitches pins masks */
static const uint8_t g_fs_masks[] = {
	_BV(PC0),
	_BV(PC1),
	_BV(PC2),
	_BV(PC3),
};

/* MIDI cc numbers for each footswitch */
static const uint8_t g_midi_cc[] = {
	102,
	103,
	104,
	105,
};

/* sync push button */
#define SYNC_BTN_PRESENT /* comment this to disable */
#ifdef SYNC_BTN_PRESENT
static const volatile uint8_t* g_sync_ddr = &DDRC;
static const volatile uint8_t* g_sync_pin = &PINC;
static const uint8_t g_sync_mask = _BV(PC4);
#endif /* SYNC_BTN_PRESENT */

#endif /* _CONFIG_H */
