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

#include "config.h"

#define MIDI_CHANNEL		1
#define MIDI_ON_CC_VAL		0
#define MIDI_OFF_CC_VAL		127

/* Individual footswitch configurations (as many as `CFG_NB_FS`) */
const struct cfg_fs g_cfg[] = {
	{
		.sw = {
			.is_momentary = true,
			.ddr = &DDRC,
			.pin = &PINC,
			.pos = PC0,
		},
		.midi = {
			.channel = MIDI_CHANNEL,
			.cc_number = 102,
			.on_cc_val = MIDI_ON_CC_VAL,
			.off_cc_val = MIDI_OFF_CC_VAL,
		},
		.led = {
			.is_enabled = true,
			.ddr = &DDRB,
			.port = &PORTB,
			.pos = PB5,
		},
	},
	{
		.sw = {
			.is_momentary = true,
			.ddr = &DDRC,
			.pin = &PINC,
			.pos = PC1,
		},
		.midi = {
			.channel = MIDI_CHANNEL,
			.cc_number = 103,
			.on_cc_val = MIDI_ON_CC_VAL,
			.off_cc_val = MIDI_OFF_CC_VAL,
		},
		.led = {
			.is_enabled = true,
			.ddr = &DDRB,
			.port = &PORTB,
			.pos = PB4,
		},
	},
	{
		.sw = {
			.is_momentary = true,
			.ddr = &DDRC,
			.pin = &PINC,
			.pos = PC2,
		},
		.midi = {
			.channel = MIDI_CHANNEL,
			.cc_number = 104,
			.on_cc_val = MIDI_ON_CC_VAL,
			.off_cc_val = MIDI_OFF_CC_VAL,
		},
		.led = {
			.is_enabled = true,
			.ddr = &DDRB,
			.port = &PORTB,
			.pos = PB3,
		},
	},
	{
		.sw = {
			.is_momentary = true,
			.ddr = &DDRC,
			.pin = &PINC,
			.pos = PC3,
		},
		.midi = {
			.channel = MIDI_CHANNEL,
			.cc_number = 105,
			.on_cc_val = MIDI_ON_CC_VAL,
			.off_cc_val = MIDI_OFF_CC_VAL,
		},
		.led = {
			.is_enabled = true,
			.ddr = &DDRB,
			.port = &PORTB,
			.pos = PB2,
		},
	},
};
