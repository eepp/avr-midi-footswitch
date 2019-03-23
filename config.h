/*
 * Copyright © 2013, 2019 Philippe Proulx <eepp.ca>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _CONFIG_H
#define _CONFIG_H

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef F_CPU
# error Please define F_CPU.
#endif

/* Footswitch debounce time (ms) */
#define CFG_DEBOUNCE_TIME_MS	10

/* Number of footswitches */
#define CFG_NB_FS		4

/* UART configuration depending on your specific MCU */
#define CFG_UART_ENABLE_TX_REG	UCSR0B
#define CFG_UART_ENABLE_TX_POS	TXEN0
#define CFG_UART_BAUD_RATE_REG	UBRR0
#define CFG_UART_TX_DDR		DDRD
#define CFG_UART_TX_POS		PD1
#define CFG_UART_TX_READY_REG	UCSR0A
#define CFG_UART_TX_READY_POS	UDRE0
#define CFG_UART_SEND_REG	UDR0
#define CFG_UART_BAUD_RATE_VAL	((F_CPU / (16 * 31250UL)) - 1)

/* Enable boot LED animation (undefine to disable) */
#define CFG_BOOT_LED_ANIMATION

/* Specific footswitch configuration structure (see `config.c`) */
struct cfg_fs {
	/* Switch configuration */
	struct {
		/*
		 * Is a momentary switch or not.
		 *
		 * If your switch makes a huge CLICK when pressing it
		 * then it's probably NOT a momentary switch.
		 */
		const bool is_momentary;

		/* Direction register */
		volatile uint8_t *const ddr;

		/* Input pin */
		volatile uint8_t *const pin;

		/* Input pin position */
		const uint8_t pos;
	} sw;

	/* MIDI configuration */
	struct {
		/* Channel (1 to 16) */
		const uint8_t channel;

		/* CC number to use */
		const uint8_t cc_number;

		/*
		 * Footswitch's "on" and "off" MIDI CC values (0 to 127)
		 * to send.
		 *
		 * Note that footswitches are mostly used to _bypass_
		 * virtual stomp boxes. Most applications assume that a
		 * full MIDI CC value (127) means "bypass", which means
		 * the stomp box is bypassed (off) when sending 127 and
		 * on when sending 0.
		 */
		const uint8_t on_cc_val;
		const uint8_t off_cc_val;
	} midi;

	/* LED configuration */
	struct {
		/* Has LED or not */
		const bool is_enabled;

		/* Direction register */
		volatile uint8_t *const ddr;

		/* Output port */
		volatile uint8_t *const port;

		/* Output port position */
		const uint8_t pos;
	} led;
};

#endif /* _CONFIG_H */
