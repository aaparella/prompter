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
        char* title;
        struct tm timestamp;
    } RSSFeed;
    
    
    RSSFeed* parseFeed(ArgumentStruct* args);               // Prase RSS data
    void displayFeed(RSSFeed* feed, ArgumentStruct* args);  // Display feed
    void freeFeed(RSSFeed* feed);                           // Free feed
    int storeSettings(ArgumentStruct* args, RSSFeed* feed);
    
#endif
