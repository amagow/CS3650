CFLAGS := -g

all: tokens

tokens: tokens.o svec.o
	gcc $(CFLAGS) -o $@ $^ -lbsd

tokens.o: tokens.c svec.h
	gcc $(CFLAGS) -c -o $@ $<

svec.o: svec.c svec.h
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -rf tmp tokens *.o

.PHONY: all clean

