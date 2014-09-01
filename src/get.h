#ifndef GET_H
    #define GET_H
		
	struct tm;	
		
    // Define struct to hold response
    typedef struct {
        char* contents;
        size_t size;
    } MessageStruct;
    
    int get(MessageStruct* message, char* url);       // Send get HTTP request to specified URL
    int storeFeed(MessageStruct message, char* filepath, char* directory); // Store message contents to temp file
    struct tm* getTime(MessageStruct* message, char* url);
#endif