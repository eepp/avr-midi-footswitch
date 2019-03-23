/*
 * Simple MIDI footswitch for AVR microcontrollers
 *
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

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"

/* Debounce time as a floating point number */
#define DEB_TIME_DBL	((double) CFG_DEBOUNCE_TIME_MS)

/* Current state of a single footswitch */
struct fs {
	bool is_on;

	/*
	 * For a momentary switch, this is the last value of the actual
	 * switch, so as to know when it's pressed and released.
	 */
	bool last_sw_is_on;
};

extern const struct cfg_fs g_cfg[];
static struct fs g_fs[CFG_NB_FS];

static void init_uart(void)
{
	/* Enable transmitter */
	CFG_UART_ENABLE_TX_REG |= _BV(CFG_UART_ENABLE_TX_POS);

	/* Set MIDI baud rate */
	CFG_UART_BAUD_RATE_REG = (CFG_UART_BAUD_RATE_VAL);
}

static void init_io(void)
{
	/* Set UART's transmitter pin as output */
	CFG_UART_TX_DDR |= _BV(CFG_UART_TX_POS);

	/* Set footswitches's pin directions */
	for (uint8_t i = 0; i < CFG_NB_FS; ++i) {
		const struct cfg_fs * const cfg = &g_cfg[i];

		/* Switch: input */
		*cfg->sw.ddr &= ~_BV(cfg->sw.pos);

		/* LED: output */
		if (cfg->led.is_enabled) {
			*cfg->led.ddr |= _BV(cfg->led.pos);
		}
	}
}

static inline void wait_uart_tx_ready(void)
{
	while (!(CFG_UART_TX_READY_REG & _BV(CFG_UART_TX_READY_POS)));
}

static inline void uart_send_byte(const uint8_t byte)
{
	wait_uart_tx_ready();
	CFG_UART_SEND_REG = byte;
}

static void send_midi_cc(const struct cfg_fs * const cfg, const bool is_on)
{
	/* MIDI status */
	uart_send_byte(0xb0 | ((cfg->midi.channel - 1) & 0xf));

	/* MIDI CC number */
	uart_send_byte(cfg->midi.cc_number);

	/* MIDI CC value */
	uart_send_byte(is_on ? cfg->midi.on_cc_val : cfg->midi.off_cc_val);
}

static inline void light_led(const struct cfg_fs * const cfg, const bool is_on)
{
	if (!cfg->led.is_enabled) {
		return;
	}

	if (is_on) {
		*cfg->led.port |= _BV(cfg->led.pos);
	} else {
		*cfg->led.port &= ~_BV(cfg->led.pos);
	}
}

static void sync_fs_outputs(const struct cfg_fs * const cfg,
	const struct fs * const fs)
{
	light_led(cfg, fs->is_on);
	send_midi_cc(cfg, fs->is_on);
}

static inline bool is_switch_on(const struct cfg_fs * const cfg)
{
	return (bool) (*cfg->sw.pin & _BV(cfg->sw.pos));
}

#ifdef CFG_BOOT_LED_ANIMATION
static void boot_led_animation(void)
{
	uint8_t i;

	for (i = 0; i < CFG_NB_FS; ++i) {
		light_led(&g_cfg[i], true);
		_delay_ms(150.);
	}

	for (i = 0; i < CFG_NB_FS; ++i) {
		light_led(&g_cfg[i], false);
		_delay_ms(150.);
	}
}

static void initial_delay(void)
{
}
#else
static void boot_led_animation(void)
{
}

static void initial_delay(void)
{
	_delay_ms(50.);
}
#endif /* CFG_BOOT_LED_ANIMATION */

static void init(void)
{
	init_io();
	boot_led_animation();
	init_uart();

	/* Just in case: make sure the circuit is up and running */
	initial_delay();

	/* Read current switch values to set the initial states */
	for (uint8_t i = 0; i < CFG_NB_FS; ++i) {
		if (!g_cfg[i].sw.is_momentary) {
			g_fs[i].is_on = is_switch_on(&g_cfg[i]);
		}
	}

	/* Synchronize outputs with states */
	for (uint8_t i = 0; i < CFG_NB_FS; ++i) {
		sync_fs_outputs(&g_cfg[i], &g_fs[i]);
	}
}

static void check_and_update_fs(const uint8_t index)
{
	const struct cfg_fs * const cfg = &g_cfg[index];
	struct fs * const fs = &g_fs[index];

	/* Initial read */
	bool sw_is_on = is_switch_on(cfg);

	if (cfg->sw.is_momentary) {
		/*
		 * For a momentary switch, the switch's value is not the
		 * same as the footswitch's state, because the switch
		 * needs to be released to be considered again for a
		 * press.
		 */
		if (sw_is_on != fs->last_sw_is_on) {
			/* Debounce */
			_delay_ms(DEB_TIME_DBL);
			sw_is_on = is_switch_on(cfg);

			if (sw_is_on != fs->last_sw_is_on) {
				/* Confirmed: set as switch's last value */
				fs->last_sw_is_on = sw_is_on;

				if (sw_is_on) {
					/* Press: toggle footswitch's state */
					fs->is_on = !fs->is_on;
					sync_fs_outputs(cfg, fs);
				}
			}
		}
	} else {
		/*
		 * For an on-off switch, the footswitch's state is the
		 * same as the switch's value.
		 */
		if (sw_is_on != fs->is_on) {
			/* Debounce */
			_delay_ms(DEB_TIME_DBL);
			sw_is_on = is_switch_on(cfg);

			if (sw_is_on != fs->is_on) {
				/* Confirmed: set footswitch's state */
				fs->is_on = sw_is_on;
				sync_fs_outputs(cfg, fs);
			}
		}
	}
}

static void run(void)
{
	for (;;) {
		for (uint8_t i = 0; i < CFG_NB_FS; ++i) {
			check_and_update_fs(i);
		}
	}
}

int main(void) {
	init();
	run();

	/* never reached lol */
	return EXIT_SUCCESS;
}
