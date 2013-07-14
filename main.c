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

/* update MIDI cc values period (prescaler: /1024) */
#define UPD_MIDI_CC_PER		(F_CPU / 1024 / 4)

/* UBRR value depending on MIDI baud rate and MCU freq. */
#define UBRR_VAL		((F_CPU / (16 * 31250)) - 1)

/* last footswitches values (must initialize) */
static uint8_t g_fs_last[NB_FS];

/* MIDI status byte */
static uint8_t g_midi_status_byte = 0xb0 | MIDI_CHAN;

static inline uint8_t read_fs(uint8_t index) {
	return *(g_fs_pins[index]) & g_fs_masks[index];
}

static void init_uart(void) {
	/* enable transmitter */
	UCSR0B |= _BV(TXEN0);
	
	/* set MIDI baud rate */
	UBRR0 = 31;
}

static void init_io(void) {
	uint8_t x;
	
	/* set Tx as output */
	DDRD |= _BV(PD1);
	
	/* set footswitch pins as inputs */
	for (x = 0; x < NB_FS; ++x) {
		*(g_fs_ddrs[x]) &= ~(g_fs_masks[x]);
	}
}

static void init_fs(void) {
	uint8_t x;
	
	/* read current footswitches */
	for (x = 0; x < NB_FS; ++x) {
		g_fs_last[x] = read_fs(x);
	}
}

static void init(void) {
	/* init. inputs/outputs */
	init_io();
	
	/* init. footswitches */
	init_fs();
	
	/* init UART for MIDI transmission */
	init_uart();
}

static inline void wait_uart_tx_ready(void) {
	while (!(UCSR0A & _BV(UDRE0)));
}

static inline void uart_send(uint8_t byte) {
	UDR0 = byte;
}

static void send_cc(uint8_t index, uint8_t on) {
	/* MIDI status */
	wait_uart_tx_ready();
	uart_send(g_midi_status_byte);
	
	/* MIDI cc */
	wait_uart_tx_ready();
	uart_send(g_midi_cc[index]);
	
	/* 0 (off) or 127 (on) */
	wait_uart_tx_ready();
	uart_send(on ? 127 : 0);
}

static void footswitch(void) {
	uint8_t x, s;
	
	for (;;) {
		for (x = 0; x < NB_FS; ++x) {
			if (read_fs(x) != g_fs_last[x]) {
				/* debounce */
				_delay_ms(10.0);
				s = read_fs(x);
				if (s != g_fs_last[x]) {
					g_fs_last[x] = s;
					send_cc(x, s);
				}
			}
		}
	}
}

int main(void) {
	init();
	footswitch();

	return 0;
}
