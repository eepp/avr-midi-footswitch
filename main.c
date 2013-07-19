/**
 * Simple MIDI footswitch for AVR microcontrollers
 * 
 * Copyright (c) 2013 Philippe Proulx <eepp.ca>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Philippe Proulx.  The name of Philippe Proulx
 * may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include "config.h"

/* UBRR value depending on MIDI baud rate and MCU freq. */
#define UBRR_VAL		((F_CPU / (16 * 31250UL)) - 1)

/* last footswitches values (must initialize) */
static uint8_t g_fs_last[NB_FS];

#ifdef SYNC_BTN_PRESENT
/* last sync button state (0 means released) */
static uint8_t g_sync_last = 0;
#endif /* SYNC_BTN_PRESENT */

/* MIDI status byte */
static const uint8_t g_midi_status_byte = 0xb0 | MIDI_CHAN;

static inline uint8_t read_fs(uint8_t index) {
	return *(g_fs_pins[index]) & g_fs_masks[index];
}

#ifdef SYNC_BTN_PRESENT
static inline uint8_t read_sync(void) {
	return *g_sync_pin & g_sync_mask;
}
#endif /* SYNC_BTN_PRESENT */

static void init_uart(void) {
	/* enable transmitter */
	UCSR0B |= _BV(TXEN0);
	
	/* set MIDI baud rate */
	UBRR0 = UBRR_VAL;
}

static void init_io(void) {
	uint8_t x;
	
	/* set Tx as output */
	DDRD |= _BV(PD1);
	
	/* set footswitch pins as inputs */
	for (x = 0; x < NB_FS; ++x) {
		*(g_fs_ddrs[x]) &= ~(g_fs_masks[x]);
	}
	
#ifdef SYNC_BTN_PRESENT
	/* set sync button as input */
	*g_sync_ddr &= ~g_sync_mask;
#endif /* SYNC_BTN_PRESENT */
}

static void init_fs(void) {
	uint8_t x;
	
	/* read current footswitches */
	for (x = 0; x < NB_FS; ++x) {
		g_fs_last[x] = read_fs(x);
	}
}

static inline void wait_uart_tx_ready(void) {
	while (!(UCSR0A & _BV(UDRE0)));
}

static inline void uart_send(uint8_t byte) {
	wait_uart_tx_ready();
	UDR0 = byte;
}

static void send_cc(uint8_t index, uint8_t on) {
	/* MIDI status */
	uart_send(g_midi_status_byte);
	
	/* MIDI cc */
	uart_send(g_midi_cc[index]);
	
	/* 0 (off) or 127 (on) */
	uart_send(on ? FS_ON_VAL : FS_OFF_VAL);
}

static void send_all_cc(void) {
	uint8_t x;
	
	for (x = 0; x < NB_FS; ++x) {
		send_cc(x, g_fs_last[x]);
	}
}

static void init(void) {
	/* init. inputs/outputs */
	init_io();
	
	/* init. footswitches */
	init_fs();
	
	/* init. UART for MIDI transmission */
	init_uart();
	
	/* send initial MIDI cc values on power on */
	_delay_ms(100.0);
	send_all_cc();
}

static void footswitch(void) {
	uint8_t x, s;
	
	for (;;) {
		for (x = 0; x < NB_FS; ++x) {
			if (read_fs(x) != g_fs_last[x]) {
				/* debounce */
				_delay_ms(SW_DEB_TIME);
				s = read_fs(x);
				if (s != g_fs_last[x]) {
					/* footswitch is toggled */
					g_fs_last[x] = s;
					send_cc(x, s);
				}
			}
		}

#ifdef SYNC_BTN_PRESENT
		/* sync button */
		if (read_sync() != g_sync_last) {
			/* debounce */
			_delay_ms(SW_DEB_TIME);
			s = read_sync();
			if (s != g_sync_last) {
				g_sync_last = s;
				if (s) {
					/* sync button is down */
					send_all_cc();
				}
			}
		}
#endif /* SYNC_BTN_PRESENT */
	}
}

int main(void) {
	init();
	footswitch();

	/* never reached lol */
	return 0;
}
