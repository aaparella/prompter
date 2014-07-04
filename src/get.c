#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include "get.h"

/**
 * Writes response from curl command to response struct
 * Called multiple times, appends new response to previous data 
 */ 
static size_t write_data(void *response, size_t size, size_t nmemb, MessageStruct *message) {

    // Calculate response of THIS PORTION OF MESSAGE and resize contents
    size_t responseSize = size * nmemb;
    message->contents = realloc(message->contents, message->size + responseSize + 1);
    
    // Copy in response at end of message
    if (message->contents)
        memcpy(message->contents + message->size, response, responseSize);
    else {
        printf("Reallocation failed\n");
        return -1;
    }
	
    // Update message size and null terminate
    message->size += responseSize;
    message->contents[message->size] = 0; // null terminate message

    // Return bytes written
    return responseSize;
}

/**
 * Perform curl request
 * Stores response from request in response paramter
 */
int get(MessageStruct* response, char* url) {
	
    CURL* curl;  
    CURLcode res;
		
    // Initialize response
    response->size = 0;
    response->contents = malloc(1);
	
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

/**
 * Store response in temporary file
 * Use pre-processor directives to determine where to store
 * response
 */
int store(MessageStruct response, char* filepath) {
	
    FILE* fp = fopen(filepath, "w+r");
	
    // If could open file, or create it, write to it
    if (fp)
        fprintf(fp, "%s\n", response.contents);
    // if not, indicate failure
    else 
        return 1;
	
    return 0;
}