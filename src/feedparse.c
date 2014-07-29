#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libxml/xmlreader.h>
#include <sys/ioctl.h>
#include <curses.h>

#include "feedparse.h"
#include "argparse.h"
#include "htmlparse.h"

#define TITLE_COLOR 1
#define AUTHOR_COLOR 2
#define STORY_COLOR 3
#define MENU_COLOR 4
#define STANDARD_COLOR 5

/** 
 * Parse conents of a story
 * Have to handle the special HTML tags specially
 */
char* parseContent(xmlDocPtr doc, xmlNodePtr contentRoot) {
    xmlChar* rawStory = xmlNodeListGetString(doc, contentRoot->xmlChildrenNode, 1);
    
    
    rawStory = (xmlChar *) ParseHtml((char *) rawStory);
    // Parse HTML by hand
    // Oh joy
     
    return (char *) rawStory;
}


/**
 * Print horizontal bar
 * No need for newline, as it goes to the end of the window and wraps around
 */
void PrintBar(struct winsize window) {
    for (int i = 0; i < window.ws_col; i++)
        printw("=");
}


/**
 * Parse individual story
 * Internal use only, not in header
 */
ArticleStruct* parseStory(xmlDocPtr doc, xmlNodePtr storyRoot) {
    // Extract basic information from each story
    xmlNodePtr cur = storyRoot->xmlChildrenNode;
    xmlChar* key;
    
    // Initialize article
    ArticleStruct* article = malloc(sizeof(ArticleStruct));
    article->title     = NULL;
    article->author    = NULL;
    article->published = NULL;
    article->story     = NULL;
    article->unread    = 1;
    
    // While we have subelements to go through
    while(cur) {
        // Example : If we find the article title
        key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
        
        // If we have a valid key
        if (key) {
            // Check if it was title
            if (!xmlStrcmp(cur->name, (xmlChar *) "title")) {
                article->title = malloc(strlen((char *) key));
                strcpy(article->title, (char *) key);
                // article->published[strlen((char *) key)] = '\0';
            }
            // Check if it was the published timestamp
            else if (!xmlStrcmp(cur->name, (xmlChar *) "published")) {
                article->published = malloc(strlen((char *) key));
                strcpy(article->published, (char *) key);
                // article->published[strlen((char *) key)] = '\0';
            }
            else if (!xmlStrcmp(cur->name, (xmlChar *) "content")) {
                // Because HTML elements are in the story, parse it specially
                article->story = parseContent(doc, cur);
            }
            // If it's the author, descend a level and find name
            else if (!xmlStrcmp(cur->name, (xmlChar *) "author")) {
                xmlNodePtr temp = cur->xmlChildrenNode;
                xmlChar* tempKey;
                
                // While we have a subtag
                while(temp) {
                    tempKey = xmlNodeListGetString(doc, temp->xmlChildrenNode, 1);
                    
                    if (tempKey) {
                        // If it's the authors name store it
                        if (!xmlStrcmp(temp->name, (xmlChar *) "name")) {
                            article->author = malloc(strlen((char *) tempKey));
                            strcpy(article->author, (char *) tempKey);
                            // article->published[strlen((char *) tempKey)] = '\0';
                        }
                    }
                    
                    // Advance to next subtag
                    temp = temp->next;	
                }
                
                // If we ever found it, free the key
                if (tempKey)
                    xmlFree(tempKey);
            }
            
            // If we found a key, free it
            if (key)
                xmlFree(key);
        }
        
        // Advance through elements
        cur = cur->next;
    }
    
    return article;
}




/**
 * Display article's contents using ncurses library
 */
void displayNCurses(ArticleStruct* article, struct winsize window) {
    
    // Print article contents to ncurses window
    clear();
    
    if (article) {
        PrintBar(window);
        
        if (article->title) {
            attron(COLOR_PAIR(TITLE_COLOR));
            printw("%s\n", article->title);
            attroff(COLOR_PAIR(TITLE_COLOR));
        }
        if (article->author) {
            attron(COLOR_PAIR(AUTHOR_COLOR));
            printw("%s\n", article->author);
            attroff(COLOR_PAIR(AUTHOR_COLOR));
        }
        if (article->published) {
            printw("%s\n", article->published);
        }
        
        PrintBar(window);
        
        if (article->story) {
            printw("\n%s\n", article->story);
        }
    }	
    
    refresh();	
    getch();		
}


/**
 * Display list of articles using ncurses library
 */
void displayFeed(ArticleStruct** articles) {
    
    int option = 1, index = 0;
    
    initscr();
    start_color();
    
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    
    
    // Set up ncurses color pairs
    init_pair(TITLE_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(AUTHOR_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(STANDARD_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(MENU_COLOR, COLOR_CYAN, COLOR_BLACK);
    
    while(1) {
        
        // Set menu color
        attron(COLOR_PAIR(MENU_COLOR));
        clear();
        
        // Output heading
        PrintBar(window);
        printw("Prompter v 0.0.1\n");
        PrintBar(window);

        if (articles) {
            index = 0;
            
            // While we have an article to print
            while(articles[index]) {  
  
                // If it's already read then change the color back to standard
                if (!articles[index]->unread)
                    attron(COLOR_PAIR(STANDARD_COLOR));
                  
                // Print out title                
                if (strlen(articles[index]->title) < window.ws_col - 6)
                    // If the title will fit in full, display it
                    printw("%3d : %s\n", index + 1, articles[index]->title);
                else 
                    // If not, print out the part that will fit and an ellipsis
                    printw("%3d : %.*s...\n", \
                        index + 1, window.ws_col - 10, articles[index]->title);
                
                // Restore color
                attron(COLOR_PAIR(MENU_COLOR));
                
                index++;
            }
        }
        
        attroff(COLOR_PAIR(MENU_COLOR));
        attron(COLOR_PAIR(STANDARD_COLOR));
        
        move(window.ws_row - 2, 0);
        PrintBar(window);
        
        // Prompt for selection and read it in
        printw("Selection (0 to quit) > ");   
        refresh();
         
        scanw("%d", &option);
    
        // When selection is 0 exit
        if (option == 0)
            break;
        
        displayNCurses(articles[option-1], window);
        articles[option-1]->unread = 0;
    }
    
    attroff(COLOR_PAIR(MENU_COLOR));
    clear();
    refresh();
}


/**
 * Free dynamically allocated article
 */
void freeArticle(ArticleStruct* article) {

    // If non-null, free each allocated field
    if (article) {
        if (article->title)
            free(article->title);
        if (article->author)
            free(article->author);
        if (article->published)
            free(article->published);	
        if (article->story)	
            free(article->story);

        free(article);
    }
}


/**
 * Free dynamically allocated array of articles
 */
void freeArticles(ArticleStruct ** articles) {

    if (articles) {
        int index = 0;

        while(1) {
            if (!articles[index]) 
                break;

            freeArticle(articles[index]);

            index++;
        }

        free(articles);
    }
}


/**
 * Parse through feed XML using libxml2 library
 * Extensively commented for educational purposes
 */
ArticleStruct** parseFeed(ArgumentStruct* args) {
    // Doc -> points to the root XML element
    // Cur -> points to current XML element
    xmlDocPtr doc;
    xmlNodePtr cur;
        
    // Keep track of number of articles
    int articlesToFetch = args->articleCount;
    int printAll = 0, articleCount = 0, arraySize = 10;
    
    if (articlesToFetch == -1)
        printAll = 1;
    
    // Array of articles parsed out of feed
    ArticleStruct** articles = malloc (arraySize * sizeof(ArticleStruct*));
    for (int i = 0; i < arraySize; i++)
        articles[i] = NULL;
    
    // Parse file
    doc = xmlReadFile(args->dataFile, NULL, XML_PARSE_RECOVER);
    
    // doc is null if file could not be parsed
    if (doc) {
        // Get the root of the parse tree
        cur = xmlDocGetRootElement(doc);
    
        // Because we got tree root, descend one level
        if (cur) {
            cur = cur->xmlChildrenNode;
            
            while(cur && (articlesToFetch || printAll)) {	

                // If we find a story, parse it, add it to array
                if (!xmlStrcmp(cur->name, (xmlChar *) "entry")) {
                    ArticleStruct* article = parseStory(doc, cur);
                    articles[articleCount++] = article;
                    
                    // Check if we have to resize
                    if (articleCount == arraySize) {
                        articles = realloc( \
                            articles, sizeof(ArticleStruct*) * arraySize * 2);
                        arraySize *= 2;
                        
                        // Allocate memory for all of the new articles
                        for (int i = articleCount; i < arraySize; i++)
                            articles[i] = NULL;
                    }
                    
                    // Feteched and article, update count
                    articlesToFetch--;
                }
                
                // Advance through XML tree
                cur = cur->next;	
            }
        }
    }
 
    return articles;
}
