all: max

# CC := clang

max: max.c
	${CC} -o $@ $(shell pwd)/$<

clean:
	rm -f max

test: max
	./max
