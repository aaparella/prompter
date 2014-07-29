#ifndef STACK_H
    #define STACK_H
    
    typedef struct {
        char** elements;
        int count;
        int size;
    } Stack;
    
    Stack* getNewStack();
    void freeStack();
    char* push(Stack* stack, char* element);
    char* popElement(Stack* stack);
    char* topElement(Stack* stack);
    
#endif