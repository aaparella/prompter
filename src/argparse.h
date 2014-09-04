#ifndef ARGPARSE_H
    #define ARGPARSE_H

    #include <time.h>

    typedef struct {
        char* url;            // RSS feed URL
        char* tempDirectory;  // Where to write temp files
        char* settingsFile;   // Where to write settings
        char* dataFile;       // Where to write data
        
        int stdout;           // Write result to stdout or not
        int color;            // Output in color or not
        int articleCount;     // Number of articles to display
        int update;           // Get new content
        int ignoreTimestamp;
        
        struct tm lastUpdated;   // Last time the article was fetched
        char* previouslyFetchedTitle;
        char* previouslyFetchedURL;
    } ArgumentStruct;
	
    void displayUsage();                                                 // Display usage guidelines and quit	
    ArgumentStruct* getDefaultArguments();                               // Initialize default arguments
    ArgumentStruct* parseArguments(int argc, char* argv[]);              // Parse command line arguments
    void freeArgs(ArgumentStruct* args);
    int fetchingNewURL(ArgumentStruct* args);

#endif