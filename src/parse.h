#ifndef PARSE_H
	#define PARSE_H

	typedef struct {
		char* url;
	} ArgumentStruct;
	
	ArgumentStruct parseArguments(int argc, char* argv[]); // parse command line arguments
	int parseFeed();                                       // parse RSS data

#endif
