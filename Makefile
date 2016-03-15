

jsplayground: jsplayground.cpp duktape.o
	g++ -o jsplayground jsplayground.cpp -Iduktape duktape.o -lm -g


duktape.o: duktape//duktape.c duktape/duktape.h
	gcc -std=c99 duktape/duktape.c -Iduktape/ -c -o duktape.o 

clean:
	rm -fr jsplayground duktape.o


all: 
	jsplayground