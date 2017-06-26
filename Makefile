###
### @file Makefile
### @author Johannes Obermueller <johannes.obermueller@gmx.at>
### @date 06.09.2015
###
### @brief Makefile for intruder_detector
### 
### @details 
###

P = test
OBJECTFILES=$(P).o huffman.o
ELF := huffman.elf

CC = gcc
DEFS = -D_XOPEN_SOURCE=500 -D_DEFAULT_SOURCE 
#DEFS += -DENDEBUG
CFLAGS = -Wall -g -std=c99 -pedantic $(DEFS)
LIBS =
DOXYGEN = doxygen

PATMOS_HW_BASEDIR ?= /opt/zanal/patmos-src/patmos
SERIAL_PORT ?= /dev/ttyS0



all: $(ELF)

host: $(P)


$(P): $(OBJECTFILES) 
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

doxy:
	$(DOXYGEN) ../doc/Doxyfile


$(ELF): test.c huffman.c
	patmos-clang -O2 \
	  -Xgold -T$(PATMOS_HW_BASEDIR)/hardware/spm_ram.t \
	  -mpatmos-disable-vliw \
	  -DUSE_SPM \
	  -DPATMOS_ENABLE_MEASURE \
	  -o $@ $^ $(LIBS)
	  #-mpatmos-singlepath=binsearch \

%.dis: %.elf
	patmos-llvm-objdump -d $< > $@

# configure the FPGA with the Patmos processor (bootloader program)
config:
	 config_altera -b USB-Blaster $(PATMOS_HW_BASEDIR)/hardware/quartus/altde2-70/patmos.sof

# download the binary to the Patmos bootloader
download: $(ELF)
	 patserdow -v $(SERIAL_PORT) $<

# execute with the emulator
emulate: $(ELF)
	 patmos-emulator $<

clean:
	rm -fr $(ELF) *.dis $(OBJECTFILES) $(P)

.PHONY: all host clean doxy config download emulate