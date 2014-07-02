#ifndef GET_H
	#define GET_H
		
	// Define struct to hold response
	typedef struct {
		char* contents;
		size_t size;
	} MessageStruct;
	
	int get(MessageStruct* message, char* url);       // Send get HTTP request to specified URL
	int store(MessageStruct message, char* filepath); // Store message contents to temp file
		
#endif