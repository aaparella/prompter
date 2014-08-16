#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "htmlparse.h"
#include "stack.h"


/**
 * Remove leading whitespace of content
 */
char* trimWhiteSpace(char* content) {
    content = strtok(content, " \t");
    
    return (content ? content : "");
}


/**
 * Check if the tag has an accompanying closing tag
 */
int isSingletonTag(char * tag) {
    // If <tag ... />
    if (tag[strlen(tag) - 1] == '/')
        return 1;
    // If tag is hr or br
    else if (!strncmp("br", tag, 2) || !strncmp("hr", tag, 2))
        return 1;
    
    return 0;
}



/**
 * Return 1 if the two tags are of the same type
 * Return 0 if the two tags are of different types
 */
int tagCompare(char* openingTag, char* closingTag) {
    return !strncmp(openingTag, closingTag + 1, strlen(closingTag) - 1);
}

/**
 * Process HTML tag
 * Properties -> Properties of tag being checked
 * Stack -> Stack used for parsing
 */
int processTag(char* properties, Stack* stack) {
    
    // Check if a singleton tag (only one should appear)
    if (!isSingletonTag(properties)) {
        // If not a closing tag, push onto stack
        if (properties[0] != '/')
            push(stack, properties);
        else {
            // Top tag on stack should correspond to this closing tag
            char* openingTag = popElement(stack);
            if (openingTag) {
                // If opening tag corresponds to closing tag, return failure
                if (!tagCompare(openingTag, properties))
                    return 0;
                // Otherwise return success
                else
                    return 1;
            }
        }
    }
    
    // Indicate the tag is properly formatted
    return 1;
}

/**
 * Parse HTML page
 */
char* ParseHtml(char* html) {
    
    char* story = (char *) malloc(sizeof(char));
    Stack* stack = getNewStack();
    
    // Prime remainder of algorithm
    char* content = strsep(&html, "<");
    
    if (content) {
        // Chew threw first tag contents
        char* properties = strsep(&html, ">");
        
        while(properties) {
            
            // False if the tag is not properly formatted
            if (!processTag(properties, stack)) {
                // Free story and return error message
                free(story);
                return "Story not parsed correctly. Sorry :(\n";
            }
                
            // Find content
            content = strsep(&html, "<");

            // Allocate space for content 
            story = realloc(story, strlen(story) + strlen(content) + 1);
            if (!story) {
                printf("ERROR : Error reallocating memory\n");
                return NULL;
            }
                
            // Concatenate the two together and null terminate the string
            // content = trimWhiteSpace(content);
            strcat(story, content);
            story[strlen(story)] = '\0';
                
            // Get properties of next tag
            properties = strsep(&html, ">");
        }
    }
    
    return story;
}

