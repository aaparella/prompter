#ifndef PARSE_H
	#define PARSE_H

	typedef struct {
		char* url;  // RSS feed URL
		int stdout; // Write result to stdout or not
	} ArgumentStruct;
	
	ArgumentStruct* getDefaultArguments();                  // Initialize default arguments
	ArgumentStruct* parseArguments(int argc, char* argv[]); // parse command line arguments
	int parseFeed();                                       // parse RSS data

#endif
