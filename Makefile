# this Makefile builds avr-midi-footswitch.
#
# "make program" to program the chip using AVRDUDE.
#
# author:	Philippe Proulx <eepp.ca>

# sources
C_SRC = main.c config.c
H_SRC = config.h

# target MCU
MCU = atmega88p
F_CPU = 8000000UL

# targets
TARGET = avr-midi-footswitch
TARGET_OUT = $(TARGET).out
TARGET_HEX = $(TARGET).hex
TARGET_MAP = $(TARGET).map

# configuration
HEXFORMAT = ihex
OPTLEVEL = s

# tools
CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
PROG = avrdude
PROGFLAGS = -c usbasp -p $(MCU) -e
RM = rm -f
AR = avr-ar

# shorts
O_FILES = $(subst .c,.o,$(C_SRC))

# C flags
CFLAGS = -mmcu=$(MCU) -O$(OPTLEVEL) -fshort-enums \
	-funsigned-bitfields -funsigned-char -Wall -DF_CPU=$(F_CPU)

# linker flags
LDFLAGS = -Wl,-Map,$(TARGET_MAP) -mmcu=$(MCU)

# phony targets
.PHONY: clean all program

# main target
all: $(TARGET_OUT)

# make output
$(TARGET_OUT): $(O_FILES)
	$(CC) $(LDFLAGS) -o $@ $(O_FILES) && \
	$(SIZE) $(TARGET_OUT)

# make binary image
$(TARGET_HEX): $(TARGET_OUT)
	$(OBJCOPY) -j .text -j .data -O $(HEXFORMAT) $< $@

# program target
program: $(TARGET_HEX)
	$(PROG) $(PROGFLAGS) -U flash:w:$(TARGET_HEX)

# objects from C files
%.o : %.c $(H_SRC)
	$(CC) $(CFLAGS) -o $@ -c $<

# clean target
clean:
	$(RM) $(TARGET_OUT) $(TARGET_HEX) $(TARGET_MAP) $(O_FILES)
