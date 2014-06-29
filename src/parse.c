#include <stdio.h>
#include "parse.h"

ArgumentStruct parseArguments(int argc, char* argv[]) {
	ArgumentStruct args;
	args.url = "http://www.theverge.com/rss/frontpage";
	
	return args;	
}

int parseFeed();