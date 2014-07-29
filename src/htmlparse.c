#include <stdio.h>
#include <string.h>

#include "htmlparse.h"
#include "stack.h"

void ParseHtml(char* html) {
    
    Stack* stack = getNewStack();
    printf("PARSING : %s\n", html);
    
    char* test = strtok(html, "<");
    printf("TEST : %s\n", test);
    
    if (test) {
        char* properties = strtok(NULL, ">");
        
        while(properties) {
            
            // Needs fleshing out
            if (properties[strlen(properties) - 1] != '/')
                push(stack, properties);
            
            test = strtok(NULL, "<");
            if (!properties)
                break;
            
            properties = strtok(NULL, ">");
        }
    }
    
    while((test = popElement(stack)))
        printf("%s\n", test);
    
}