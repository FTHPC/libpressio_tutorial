CXXFLAGS=-g $(shell pkg-config --cflags libpressio_cxx)
LDLIBS=$(shell pkg-config --libs libpressio_cxx)
ALL=libpressio_typeswitch_base.so libpressio_typeswitch_solution.so test_pressio_base
all:$(ALL)
lib%.so:%.cc
	$(CXX) $(CXXFLAGS) -fPIC -shared -o $@ $^
test_pressio_base: libpressio_typeswitch_base.so
test_pressio_base: LDFLAGS+=-L$(shell pwd) -l pressio_typeswitch_base 
check: $(ALL)
	./test_pressio_base
clean:
	$(RM) $(ALL)
