###
### @file Makefile
### @author Johannes Obermueller <johannes.obermueller@gmx.at>
### @date 06.09.2015
###
### @brief Makefile for intruder_detector
### 
### @details 
###

TARGET = test
SRCS = test.c huffman.c stack.c
OBJECTFILES = test.o huffman.o stack.o
ANALYSIS_ENTRY = encode

CC = gcc
DEFS = -D_XOPEN_SOURCE=500 -D_DEFAULT_SOURCE 
#DEFS += -DENDEBUG
CFLAGS = -Wall -g -std=c99 -pedantic $(DEFS)
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -O0
LIBS =
DOXYGEN = doxygen

# change here or set at the command line with 'make PATMOS_CONFIG=full ...'
PATMOS_CONFIG ?= local
PATMOS_CONFIG_PATH = ../configurations/config_$(PATMOS_CONFIG).pml

TOOL_CONFIG = platin tool-config -t $(TOOL) -i $(PATMOS_CONFIG_PATH)

PATMOS_HW_BASEDIR ?= /opt/zanal/patmos-src/patmos
SERIAL_PORT ?= /dev/ttyS0



all: $(TARGET).elf llvm.ais

host: $(TARGET)

$(TARGET): $(OBJECTFILES) 
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

doxy:
	$(DOXYGEN) ../doc/Doxyfile


$(TARGET).elf: TOOL = clang
$(TARGET).elf $(TARGET).elf.pml: $(SRCS)
	patmos-clang -O0 -g -o $(TARGET).elf  $(shell $(TOOL_CONFIG)) \
	  -Xgold -T$(PATMOS_HW_BASEDIR)/hardware/spm_ram.t \
	  -Xllc -mpreemit-bitcode=$(TARGET).elf.bc \
	  -mpatmos-disable-vliw \
	  -DUSE_SPM \
	  -DPATMOS_ENABLE_MEASURE \
	  -mserialize=$(TARGET).elf.pml $^ $(LIBS)
	  #-mpatmos-singlepath=binsearch
# $(TARGET).elf $(TARGET).elf.pml: $(SRCS)
# 	patmos-clang $(CFLAGS) -o $(TARGET).elf $(shell $(TOOL_CONFIG)) \
# 	    -Xllc -mpreemit-bitcode=$(TARGET).elf.bc \
# 	    -mserialize=$(TARGET).elf.pml $^
	platin extract-symbols -i $(TARGET).elf.pml -o $(TARGET).elf.pml \
	    $(TARGET).elf

llvm.ais: $(TARGET).elf.pml
	platin pml2ais --ais llvm.ais \
	    --ais-disable-exports header,mem-addresses -i $(TARGET).elf.pml

$(TARGET).sim: TOOL = pasim
$(TARGET).sim: $(TARGET).elf
	pasim -V $(shell $(TOOL_CONFIG)) $< 2> $@

%.dis: %.elf
	patmos-llvm-objdump -d $< > $@

# configure the FPGA with the Patmos processor (bootloader program)
config:
	 config_altera -b USB-Blaster $(PATMOS_HW_BASEDIR)/hardware/quartus/altde2-70/patmos.sof

# download the binary to the Patmos bootloader
download: $(TARGET).elf
	 patserdow -v $(SERIAL_PORT) $<

# execute with the emulator
emulate: $(TARGET).elf
	 patmos-emulator $<

patmos.ais: TOOL = ait
patmos.ais:
	$(TOOL_CONFIG) > $@

$(TARGET).dis: $(TARGET).elf
	patmos-llvm-objdump -d -t -fpatmos-print-bytes=call $< > $@

$(TARGET).sim: TOOL = pasim
$(TARGET).sim: $(TARGET).elf
	pasim -V $(shell $(TOOL_CONFIG)) $< 2> $@

$(TARGET).trace: TOOL = pasim
$(TARGET).trace: $(TARGET).elf
	pasim $< --debug-fmt=trace --debug=0 --debug-file=$@ \
	    $(shell $(TOOL_CONFIG)) -V

platin-wca:
	platin wcet \
	  -b $(TARGET).elf \
	  -i $(TARGET).elf.pml \
	  -i $(PATMOS_CONFIG_PATH) \
	  -i $(ANALYSIS_ENTRY).ff.pml \
	  -e $(ANALYSIS_ENTRY) \
	  --enable-wca --disable-ait

clean:
	rm -rf $(TARGET).elf *.dis *.o $(TARGET)

.PHONY: all host clean doxy config download emulate platin-wca
