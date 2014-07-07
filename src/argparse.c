#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "argparse.h"

 /**
  * Assign default argument values
  */
ArgumentStruct* getDefaultArguments() {
    ArgumentStruct* defaults = malloc(sizeof(ArgumentStruct));
    
    defaults->url = "http://www.theverge.com/rss/frontpage";
    defaults->tempDirectory = NULL;
    
    // If on a UNIX system
    #ifdef __unix__
        defaults->tempDirectory = "/tmp/prompter";
        defaults->settingsFile  = "/tmp/prompter/settings.txt";
        defaults->dataFile      = "/tmp/prompter/data.txt";
    // If being run from OS X
    #elif defined __APPLE__
        defaults->tempDirectory = "/Library/Caches/prompter";
        defaults->settingsFile  = "/Library/Caches/prompter/settings.txt";
        defaults->dataFile      = "/Library/Caches/prompter/data.txt";
    #endif

    defaults->stdout       = 1;
    defaults->color        = 1;
    defaults->articleCount = -1;
    
    return defaults;
}


/**
 * Display program usage guide
 * Also exits program, indicates success to OS
 */
void displayUsage() {
    printf("Usage : prompter {OPTION}\n\n");

    printf("-s --silent:\n");
    printf("\tOnly update, do not output results\n");

    printf("-d --display:\n");
    printf("\tSpecify number of articles to output\n");

    printf("-n --nocolor:\n");
    printf("\tDo not use color for output\n");

    printf("-f --feed:\n");
    printf("\tURL feed to use, defaults to previous URL\n");

    printf("-h --help:\n");
    printf("\tDisplay this menu\n");
	
    printf("-v --version:\n");
    printf("\tDisplay current version number\n");
	
    exit(0);
}


/**
 * Parse command line options
 * Return a configured ArgumentStruct* on success
 * Return NULL if arguments cannot be successfuly parsed
 */
ArgumentStruct* parseArguments(int argc, char* argv[]) {
    // Initially assume all arguments to be default
    ArgumentStruct* args;
    args = getDefaultArguments();
    
    // Run through each option
    // For each flag, next item listed should be the desired value
    // Will add error checking eventually
    for (int i = 1; i < argc; i++) {
        if (!strcmp("-f", argv[i]) || !strcmp("--feed", argv[i]))
            args->url = argv[++i];
        else if (!strcmp("-s", argv[i]) || !strcmp("--silent", argv[i]))
            args->stdout = 0;
        else if (!strcmp("-d", argv[i]) || !strcmp("--display", argv[i]))
            args->articleCount = atoi(argv[++i]);
        else if (!strcmp("-n", argv[i]) || !strcmp("--nocolor", argv[i]))
            args->color = 0;
        else if (!strcmp("-v", argv[i]) || !strcmp("--version", argv[i])) {
            printf("Prompter 0.0.1\n");
            if (args)
                free(args);
        
            exit(0);
        }
        // Only other option is either -h or an invalid option
        // In either case, displayUsage() is called
        else
            displayUsage();
    }
    
    return args;	
}

/**
 * Store arguments used for this execution
 * Creates file if it does not already exis
 */
int storeArgs(ArgumentStruct* args, char* filepath, char* directory) {
    
    // Still to be written
    
    return 0;
}