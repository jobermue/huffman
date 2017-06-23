P = huffman
OBJECTS =
CFLAGS = -g -Wall -O3
#CFLAGS += `pkg-config --cflags opencv`
LDLIBS = -lm
#LDLIBS += `pkg-config --libs opencv`
CC = c99

$(P): $(OBJECTS)

