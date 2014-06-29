#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "parse.h"
#include "get.h"
 
int main(int argc, char* argv[]) {
	
	ArgumentStruct args;
	args = parseArguments(argc, argv);

	char* url = args.url;
	MessageStruct response;
	
	if(get(&response, url))
		printf("Error in performing CURL");
	if(store(response))
		printf("Error writing response to file");
	
	return 0;
}
