DATADIR=$(realpath $(shell pwd)/../..)/datasets/
LDFLAGS=$(shell pkg-config --libs libpressio)
CFLAGS=-std=c99 -g $(shell pkg-config --cflags libpressio)
CPPFLAGS=-DDATADIR="\"$(DATADIR)\""
ALL=get_compressor_options basics list_compressors
.PHONY: all clean

all:  $(ALL)
clean:
	$(RM) $(ALL) $(wildcard *.o)
