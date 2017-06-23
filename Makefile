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

CC = gcc
DEFS = -D_XOPEN_SOURCE=500 -D_DEFAULT_SOURCE
CFLAGS = -Wall -g -std=c99 -pedantic $(DEFS)
LIBS = -lm
DOXYGEN = doxygen

OBJECTFILES=$(P).o huffman.o


all: $(P)


$(P): $(OBJECTFILES) 
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<


doxy:
	$(DOXYGEN) ../doc/Doxyfile

clean:
	rm -f $(OBJECTFILES) $(P)

.PHONY: all clean doxy
