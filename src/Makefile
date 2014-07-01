prompter : prompter.c
	gcc -g -Wall -c prompter.c

parse : parse.h parse.c
	gcc -Wall -g -c -I/usr/include/libxml2 parse.c -lxml2

get : get.c get.h
	gcc -g  -Wall -c -lcurl get.c
	
all : prompter.o parse.o get.o
	gcc -Wall -I/usr/include/libxml2 -o prompter prompter.o parse.o get.o -lxml2 -lcurl

clean :
	rm prompter *.o
