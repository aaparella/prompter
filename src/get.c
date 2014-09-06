#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

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
 * Retrieve the last time that the given URL was updated
 */
struct tm* getTime(MessageStruct* response, char* url) {
    CURL* curl;
    CURLcode res;
        
    struct tm* timestamp = malloc(sizeof(struct tm));
    
    response->size = 0;
    response->contents = malloc(1);
    
    curl = curl_easy_init();
    
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);    // Specify callback function
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);          // Specify container
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl, CURLOPT_FILETIME, 1L);
        
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perofmr() failed: %s\n", curl_easy_strerror(res));
            return NULL;
        }
        
        char* line = strstr(response->contents, "Last-Modified");
        if (line) {
            line = strtok(line, "\n");
            strptime(line, "Last-Modified: %a, %d %b %Y %X %Z", timestamp);
        }
        
        curl_easy_cleanup(curl);
    }
    
    return timestamp;
}


/**
 * Store response in temporary file
 * Use pre-processor directives to determine where to store
 * response
 */
int storeFeed(MessageStruct response, char* filepath, char* directory) {
	
    FILE* fp = fopen(filepath, "w+r");
    
    // Problem opening file, try to create it
    if (!fp) {
        mkdir(directory, S_IRWXU);
        fp = fopen(filepath, "w+r");
    }
	
    // If could open file, or create it, write to it
    if (fp)
        fprintf(fp, "%s\n", response.contents);
    // if not, indicate failure
    else 
        return 1;
        
    fclose(fp);
	
    return 0;
}


/**
 * Manually check time, because the difftime function is not working correctly
 * This function makes me hate myself sometimes
 */
int timeDiff(struct tm* beginning, struct tm* ending) {
    
    // Get time_t structs because, of course, gmtime only takes time_t structs
    time_t begtwo = mktime(beginning);
    time_t endtwo = mktime(ending);
    
    // Why oh why does this return a tm struct instead of time_t
    struct tm* beg = gmtime(&begtwo);
    struct tm* end = gmtime(&endtwo);    
    
    // This is just depressing
    time_t correctedBeginning = mktime(beg);
    time_t correctedEnding = mktime(end);
    
    return difftime(correctedBeginning, correctedEnding);
}









