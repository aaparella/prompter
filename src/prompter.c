#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

// Custom headers
#include "parse.h"
#include "get.h"


void dispUsageGuide() {
	printf("Usage : prompter [RSS feed URL]\n");
	printf("This could use some flushing out");
}
 
int main(int argc, char* argv[]) {
	
	// Default URL
	// <3 Topolsky
	char* url = "http://www.theverge.com/rss/frontpage";

	// Initialize response struct
	MessageStruct response;
	response.size = 0;
	response.contents = malloc(1);

	// If a URL has been provided
	if (argc != 1)
		url = argv[1];	

	get(&response, url);
	printf("%s", response.contents);	
	
	return 0;
}
