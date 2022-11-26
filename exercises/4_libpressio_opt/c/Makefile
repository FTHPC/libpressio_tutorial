DATADIR=$(realpath $(shell pwd)/../..)/datasets/
LDLIBS=$(shell pkg-config --libs libpressio libpressio_meta)
CFLAGS=-std=c99 -g $(shell pkg-config --cflags libpressio libpressio_meta)
CPPFLAGS=-DDATADIR="\"$(DATADIR)\""
ALL=opt_zfp_perf
.PHONY: all clean

all:  $(ALL)
check: $(ALL)
	./opt_zfp_perf
clean:
	$(RM) $(ALL) $(wildcard *.o)
