prompter.out : prompter.o parse.o get.o
	gcc -lcurl -o prompter prompter.o parse.o get.o

prompter : prompter.c
	gcc -g -c prompter.c

parse : parse.h parse.c
	gcc -g -c parse.c

get : get.c get.h
	gcc -g -c -lcurl get.c

clean :
	rm prompter *.o
