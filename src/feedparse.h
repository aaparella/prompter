#ifndef PARSE_H
    #define PARSE_H
    
    #include "argparse.h"
    
    typedef struct {
        char* published;
        char* author;
        char* title;
        char* story;
    } ArticleStruct;
    
    ArticleStruct** parseFeed(ArgumentStruct* args);                   // Parse RSS data
    void freeArticles(ArticleStruct** articles);

#endif
