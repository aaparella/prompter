#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

/**
 * Initialize stack structure
 * elements pointer initially NULL, 0 elements
 */
Stack* getNewStack() {
    
    Stack* stack = (Stack *) malloc(sizeof(Stack));
    
    stack->size = 0;
    stack->count = 0;
    stack->elements = NULL;
    
    return stack;
}


/**
 * Free dynamically allocated stack structure
 * Protects against NULL pointers
 */
void freeStack(Stack* stack) {
    if (stack) {
        if (stack->elements)
            free(stack->elements);
        
        free(stack);
    }
}


/**
 * Add element to top of stack
 */
char* push(Stack* stack, char* element) {
    
    // Initialize via the constructor
    // Could initialize here, but would be very ugly
    if(!stack)
        return NULL;
    
    // Allocate stack if needed
    if (!stack->elements) {
        stack->elements = (char **) malloc (sizeof(char*) * 10);
        stack->size = 10;
    }
            
    // Increment count
    if (stack->size == stack->count) {
        stack->elements = realloc(stack->elements, sizeof(char*) * stack->size * 2);
        stack->size *= 2;
    }
    
    stack->elements[stack->count++] = element;
    
    return stack->elements[stack->count - 1];
}

/**
 * Pop off top element of stack
 * Returns element popped, or NULL if nothing to pop
 */
char* popElement(Stack* stack) {
    if (stack) {
        if (stack->count != 0) {
            stack->count--;
            return stack->elements[stack->count];
        }
        
        return NULL;
    }
    
    return NULL;
}


/**
 * Return top element of the stack without popping it
 */
char* topElement(Stack* stack) {
    if(stack)
        if (stack->elements && stack->count != 0)
            return stack->elements[stack->count - 1];

    return NULL;
}
