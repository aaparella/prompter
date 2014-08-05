#ifndef PARSE_H
    #define PARSE_H
    
    #include "argparse.h"
    
    typedef struct {
        char* published;
        char* author;
        char* title;
        char* story;
        
        int unread;
    } ArticleStruct;
    
    
    typedef struct {
        ArticleStruct** articles;
        int articleCount;
        char* url;
    } RSSFeed;
    
    
    RSSFeed* parseFeed(ArgumentStruct* args);        // Prase RSS data
    void displayFeed(RSSFeed* feed);                 // Display feed
    void freeFeed(RSSFeed* feed);                    // Free feed
#endif
