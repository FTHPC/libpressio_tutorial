DATADIR=$(realpath $(shell pwd)/../..)/datasets/
LDFLAGS=$(shell pkg-config --libs libpressio libpressio_opt)
CFLAGS=-std=c99 -g $(shell pkg-config --cflags libpressio libpressio_opt)
CPPFLAGS=-DDATADIR="\"$(DATADIR)\""
ALL=opt_zfp_perf
.PHONY: all clean

all:  $(ALL)
check: $(ALL)
	./opt_zfp_perf
clean:
	$(RM) $(ALL) $(wildcard *.o)
