#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "get.h"

static size_t write_data(void *response, size_t size, size_t nmemb, MessageStruct *message) {

	// Calculate response of THIS PORTION OF MESSAGE and resize contents
	size_t responseSize = size * nmemb;
	message->contents = realloc(message->contents, message->size + responseSize + 1);

	// Copy in response at end of message
	if (message->contents)
		memcpy(message->contents + message->size, response, responseSize);
	else {
		printf("Reallocation failed");
		return -1;
	}
	
	// Update message size and null terminate
	message->size += responseSize;
	message->contents[message->size] = 0; // null terminate message

	// Return bytes written
	return responseSize;
}

int get(MessageStruct* response, char* url) {
	
	CURL* curl;  
	CURLcode res;
	
	curl = curl_easy_init();
	
	if (curl) {
		// Set curl options
		curl_easy_setopt(curl, CURLOPT_URL, url);                     // Specify URL
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);           // Follow redirects
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);          // Specify container
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);    // Specify callback function
		
		// Perform curl command	
		res = curl_easy_perform(curl);
		
		// If curl request was NOT successful
		if(res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
			return 1;
		}
		
		// Cleanup curl environment
		curl_easy_cleanup(curl);
	}
	
	return 0;
}
