#ifndef ARGPARSE_H
    #define ARGPARSE_H

    typedef struct {
        char* url;            // RSS feed URL
        char* tempDirectory;  // Where to write temp files
        char* settingsFile;   // Where to write settings
        char* dataFile;       // Where to write data
        int stdout;           // Write result to stdout or not
        int color;            // Output in color or not
        int articleCount;     // Number of articles to display
    } ArgumentStruct;
	
    void displayUsage();                                                 // Display usage guidelines and quit
	
    ArgumentStruct* getDefaultArguments();                               // Initialize default arguments
    ArgumentStruct* parseArguments(int argc, char* argv[]);              // Parse command line arguments
    int storeArgs(ArgumentStruct* args, char* filepath, char* directry); // Store settings to temp file

#endif