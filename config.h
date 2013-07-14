#ifndef _CONFIG_H
#define _CONFIG_H

/* MIDI channel */
#define MIDI_CHAN		0

/* number of footswitches */
#define NB_FS			4

/* footswitches data direction registers */
static volatile uint8_t* g_fs_ddrs[] = {
	&DDRC,
	&DDRC,
	&DDRC,
	&DDRC,
};

/* footswitches pins */
static volatile uint8_t* g_fs_pins[] = {
	&PINC,
	&PINC,
	&PINC,
	&PINC,
};

/* footswitches pin indexes */
static uint8_t g_fs_masks[] = {
	_BV(PC0),
	_BV(PC1),
	_BV(PC2),
	_BV(PC3),
};

/* MIDI cc numbers for each footswitch */
static uint8_t g_midi_cc[] = {
	102,
	103,
	104,
	105,
};

#endif /* _CONFIG_H */
