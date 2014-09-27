#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

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
    // If being run on Windows
    #elif defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        defaults->tempDirectory = "C:\WINDOWS\TEMP\prompter";
        defaults->settingsFile  = "C:\WINDOWS\TEMP\prompter\settings.txt";
        defaults->dataFile      = "C:\WINDOWS\TEMP\prompter\data.txt";
    #endif

    defaults->stdout           = 1;
    defaults->color            = 1;  // Default to have color
    defaults->articleCount     = -1; // Default to printing all articles
    defaults->update           = 1;  // Default to fetching new content
    defaults->ignoreTimestamp  = 0;
    defaults->previouslyFetchedTitle = NULL;
    defaults->previouslyFetchedURL = NULL;
    
    return defaults;
}


/**
 * Free dynamically allocated args object
 */
void freeArgs(ArgumentStruct* args) {
    if (args) {
        free(args->previouslyFetchedTitle);
        free(args->previouslyFetchedURL);
        
        free(args);
    }
}


/**
 * Display program usage guide
 * Also exits program, indicates success to OS
 */
void displayUsage(char** argv) {
    printf("Usage : %s {OPTION}\n\n", argv[0]);

    printf("-a --all:");
    printf("\tShow all articles, ignoring whether or not they are new\n");
    
    printf("-s --silent:");
    printf("\tOnly update, do not output results\n");

    printf("-d --display:");
    printf("\tSpecify number of articles to output\n");

    printf("-n --nocolor:");
    printf("\tDo not use color for output\n");
    
    printf("-o --offline:");
    printf("\tDo not fetch new content\n");

    printf("-f --feed:");
    printf("\tURL feed to use, defaults to previous URL\n");

    printf("-h --help:");
    printf("\tDisplay this menu\n");
	
    printf("-v --version:");
    printf("\tDisplay current version number\n");
	
    exit(0);
}


/**
 * Check if we are fetching the same URL as we did last time
 */
int fetchingNewURL(ArgumentStruct* args) {
    if (args && args->previouslyFetchedURL && args->url)
        return strcmp(args->previouslyFetchedURL, args->url);
    
    return 0;
}


/**
 * Load the last settings used for comparison
 */
void loadLastSettings(ArgumentStruct* args) {
    FILE* fp = fopen(args->settingsFile, "r");
    if (fp) {
        char* line = NULL;
        size_t len = 0;
        
        getline(&line, &len, fp);
        if (len) {
            args->previouslyFetchedTitle = malloc(len);
            strcpy(args->previouslyFetchedTitle, line);
            
            if (args->previouslyFetchedTitle[strlen(line) - 1] == '\n')
                args->previouslyFetchedTitle[strlen(line) - 1] = 0;
        }
        else
            return;
        
        getline(&line, &len, fp);
        if (len) {
            args->previouslyFetchedURL = malloc(len);
            strcpy(args->previouslyFetchedURL, line);
            
            if (args->previouslyFetchedURL[strlen(line) - 1] == '\n')
                args->previouslyFetchedURL[strlen(line) - 1] = 0;
        }
        else
            return;
            
        getline(&line, &len, fp);
        if (len)
            strptime(line, "%a %b %d %X %Y", &(args->lastUpdated));
    }
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
        if (!strcmp("-a", argv[i]) || !strcmp("--all", argv[i]))
            args->ignoreTimestamp = 1;
        else if (!strcmp("-f", argv[i]) || !strcmp("--feed", argv[i]))
            args->url = argv[++i];
        else if (!strcmp("-s", argv[i]) || !strcmp("--silent", argv[i]))
            args->stdout = 0;
        else if (!strcmp("-o", argv[i]) || !strcmp("--offline", argv[i]))
            args->update = 0;
        else if (!strcmp("-d", argv[i]) || !strcmp("--display", argv[i]))
            args->articleCount = atoi(argv[++i]);
        else if (!strcmp("-n", argv[i]) || !strcmp("--nocolor", argv[i]))
            args->color = 0;
        else if (!strcmp("-v", argv[i]) || !strcmp("--version", argv[i])) {
            printf("%s 0.0.1\n", argv[0]);
            if (args)
                free(args);
        
            exit(0);
        }
        // Only other option is either -h or an invalid option
        // In either case, displayUsage() is called
        else
            displayUsage(argv);
    }
    
    loadLastSettings(args);
        
    return args;	
}