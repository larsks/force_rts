CFLAGS = -fPIC

all: force_rts.so

force_rts.so: force_rts.o
	$(LD) -shared -fPIC -o $@ $<
