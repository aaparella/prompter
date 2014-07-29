#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "htmlparse.h"
#include "stack.h"


int hasContent(char* content) {
    for (int i = 0; i < strlen(content); i++) 
        if ((content[i] != ' ') && (content[i] != '\t'))
            return 1;

    return 0;
}

char* ParseHtml(char* html) {
    
    char* story = (char *) malloc(sizeof(char));
    // Stack* stack = getNewStack();
    
    char* content = strsep(&html, "<");
    
    if (content) {
        char* properties = strsep(&html, ">");
        
        while(properties) {
            
            // Needs fleshing out
            // Will ultimately need to be used in order to validate HTML
            // Also needed ot control presentation
            // if (properties[strlen(properties) - 1] != '/')
            //     push(stack, properties);

            // Find content
            content = strsep(&html, "<");

            // Allocate space for content 
            story = realloc(story, strlen(story) + strlen(content) + 1);
            if (!story) {
                printf("ERROR : Error reallocating memory\n");
                return NULL;
            }
                
            // Concatenate the two together and null terminate the string
            strcat(story, content);
            // story[strlen(story)] = '\0';
                
            // Get properties of next tag
            properties = strsep(&html, ">");
        }
    }
    
    printf("%s\n", story);
    return story;
}

