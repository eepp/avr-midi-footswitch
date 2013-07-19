#ifndef _CONFIG_H
#define _CONFIG_H

/* MIDI channel (0 means channel 1) */
#define MIDI_CHAN	0

/* switches debounce time (ms) */
#define SW_DEB_TIME	10.0

/* number of footswitches */
#define NB_FS		4

/**
 * footswitches are mostly used to bypass stomp boxes. most softwares
 * assume that a full MIDI cc value (127) means "bypass", which means the
 * effect will be bypassed/off when sending 127 and on when sending 0.
 * the following definition selects the desired behaviour.
 */
#define BYPASS_SENDS_ZERO /* comment this line to send 127 when bypassing */
#ifdef BYPASS_SENDS_ZERO
#define FS_ON_CC_VAL	0
#define FS_OFF_CC_VAL	127
#else
#define FS_ON_CC_VAL	127
#define FS_OFF_CC_VAL	0
#endif /* BYPASS_SENDS_ZERO */

/* sync push button */
#define SYNC_BTN_PRESENT /* comment this line to disable sync button */
#ifdef SYNC_BTN_PRESENT
static volatile uint8_t* const g_sync_ddr = &DDRC;
static volatile uint8_t* const g_sync_pin = &PINC;
static const uint8_t g_sync_mask = _BV(PC4);
#endif /* SYNC_BTN_PRESENT */

/* footswitch structure */
struct fs {
	volatile uint8_t* const ddr;	/* direction register */
	volatile uint8_t* const pin;	/* input pin */
	const uint8_t mask;		/* input pin bit mask */
	const uint8_t cc;		/* cc number to use */
	uint8_t last;			/* last footswitch value */
};

/* footswitches configurations */
static struct fs g_fs[] = {
	{
		.ddr = &DDRC,
		.pin = &PINC,
		.mask = _BV(PC0),
		.cc = 102
	},
	{
		.ddr = &DDRC,
		.pin = &PINC,
		.mask = _BV(PC1),
		.cc = 103
	},
	{
		.ddr = &DDRC,
		.pin = &PINC,
		.mask = _BV(PC2),
		.cc = 104
	},
	{
		.ddr = &DDRC,
		.pin = &PINC,
		.mask = _BV(PC3),
		.cc = 105
	},
};

#endif /* _CONFIG_H */
