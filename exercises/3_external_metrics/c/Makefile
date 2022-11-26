DATADIR=$(realpath $(shell pwd)/../..)/datasets/
LDLIBS=-lm $(shell pkg-config --libs libpressio)
CFLAGS=-std=c99 -g -O2 $(shell pkg-config --cflags libpressio)
CPPFLAGS=-DDATADIR="\"$(DATADIR)\""
ALL=run_external external_metric
.PHONY: all clean

all:  $(ALL)
clean:
	$(RM) $(ALL) $(wildcard *.o)
