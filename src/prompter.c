#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "parse.h"
#include "get.h"
 
int main(int argc, char* argv[]) {
	
	ArgumentStruct* args;
	args = parseArguments(argc, argv);

	MessageStruct response;
	
	if(get(&response, args->url))
		printf("Error in performing CURL");
		
	if (!args->stdout) {
		if(store(response))
			printf("Error writing response to file");
	}
	else 
		printf("%s", response.contents);
	
	free(args);
	return 0;
}
