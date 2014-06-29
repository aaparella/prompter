#ifndef GET_H
	#define GET_H
		
	// Define struct to hold response
	typedef struct {
		char* contents;
		size_t size;
	} MessageStruct;
	
	int get(MessageStruct* message, char* url); // Send get HTTP request to specified URL
	int store(MessageStruct message);           // Store message contents to temp file
	
	// Function for parsing response from request
	static size_t write_data(void *response,
													size_t size, size_t nmemb,
													MessageStruct *message);
	
#endif