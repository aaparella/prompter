#ifndef ARGPARSE_H
    #define ARGPARSE_H

    typedef struct {
        char* url;          // RSS feed URL
        char* storagePath;  // Where to write temp files
        int stdout;         // Write result to stdout or not
        int articleCount;   // Number of articles to display
    } ArgumentStruct;
	
    void displayUsage();                                    // Display usage guidelines and quit
	
    ArgumentStruct* getDefaultArguments();                  // Initialize default arguments
    ArgumentStruct* parseArguments(int argc, char* argv[]); // Parse command line arguments

#endif