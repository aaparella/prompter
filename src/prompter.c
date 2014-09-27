/************************************+
 * Prompter.c                        *
 * Author : Alexander Parella        *
 * CLI RSS feed reader, source       *
 * available at github.com/aaparella *
 * Usage : prompter --help           *
 +************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "argparse.h"
#include "feedparse.h"
#include "get.h"
#include "stack.h"
 
int main(int argc, char* argv[]) {
	
    // Parse command line arguments, use defaults when needed
    ArgumentStruct* args;
    args = parseArguments(argc, argv);
    
    // Struct to store response from CURL
    MessageStruct response;    
    struct tm* lastUpdated = getTime(&response, args->url);
    
    // Preapre response for re-use
    freeResponse(response);
    
    // Get RSS feed from server
    if (args->update) {
        // If we're either not refetching the same URL, or it has updates
        if (args->ignoreTimestamp || fetchingNewURL(args) ||
            args->previouslyFetchedURL == NULL || timeDiff(&(args->lastUpdated), lastUpdated)) {
                
            if(get(&response, args->url)) {
                printf("ERROR : Could not find feed at %s\n", args->url);
                
                freeArgs(args);
                
                return 1;
            }
        
            // Write result out to file
            if(storeFeed(response, args->dataFile, args->tempDirectory)) {
                printf("ERROR : Writing response to file (Running again should fix this)\n");
                
                freeArgs(args);
                
                return 1;
            }
        }
        else {
            printf("No updates for feed : %s\n", args->url);
            freeArgs(args);
            
            return 0;
        }
    }
    
    
    
    // Create array of article sturctures
    RSSFeed* feed = NULL;
    feed = parseFeed(args);
    
    // Display feed
    displayFeed(feed, args);
    storeSettings(args, feed);
    
    // Free everything that has been dynamically allocated
    freeFeed(feed);
    freeResponse(response);
    freeArgs(args);
    
    return 0;
}
