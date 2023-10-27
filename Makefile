CFLAGS = -fPIC

all: force_rts.so

force_rts.so: force_rts.o
	$(LD) -shared -fPIC -o $@ $<

clean:
	rm -f force_rts.o force_rts.so
