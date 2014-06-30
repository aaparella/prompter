#include <stdio.h>
#include <stdlib.h>
#include "parse.h"


/**
 * Assign default argument values
 */
ArgumentStruct* getDefaultArguments() {
	ArgumentStruct* defaults = malloc(sizeof(ArgumentStruct));
	
	defaults->url = "http://www.theverge.com/rss/frontpage";
	defaults->stdout = 1;
	
	return defaults;
}

/**
 * Parse command line options
 * Return a configured ArgumentStruct* on success
 * Return NULL if arguments cannot be successfuly parsed
 */
ArgumentStruct* parseArguments(int argc, char* argv[]) {
	// Initially assume all arguments to be default
	ArgumentStruct* args;
	args = getDefaultArguments();
	
	// Read through options provided
	// Yeah okay whatever I'll get to this eventually
	
	return args;	
}

int parseFeed();