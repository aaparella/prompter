#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libxml/xmlreader.h>
#include <sys/ioctl.h>
#include <curses.h>
#include <sys/stat.h>
#include <time.h>

#include "feedparse.h"
#include "argparse.h"
#include "htmlparse.h"

#define TITLE_COLOR 1
#define AUTHOR_COLOR 2
#define STORY_COLOR 3
#define MENU_COLOR 4
#define STANDARD_COLOR 5


/**
 * Store arguments used for this execution, only needed settings
 * Creates file if it does not already exis
 */
int storeSettings(ArgumentStruct* args, RSSFeed* feed) {

    // Open settings file
    FILE* fp = fopen(args->settingsFile, "w+r");
    // If it does not exist, create it
    if (!fp) {
        mkdir(args->tempDirectory, S_IRWXU);
        fp = fopen(args->settingsFile, "w+r");
    }

    fprintf(fp, "%s\n%s\n%s",feed->title, feed->url, asctime(&(feed->timestamp)));

    return 0;
}


/** 
 * Parse conents of a story
 * Have to handle the special HTML tags specially
 */
char* parseContent(xmlDocPtr doc, xmlNodePtr contentRoot) {
    xmlChar* story = xmlNodeListGetString(doc, contentRoot->xmlChildrenNode, 1);
    story = (xmlChar *) ParseHtml((char *) story);
     
    return (char *) story;
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
 * Find the author's name
 */
char* findAuthor(xmlDocPtr doc, xmlNodePtr cur) {
    xmlNodePtr temp = cur->xmlChildrenNode;
    xmlChar* tempKey;
    char* author = NULL;
    
    // While we have a subtag
    while(temp) {
        tempKey = xmlNodeListGetString(doc, temp->xmlChildrenNode, 1);
        
        if (tempKey) {
            // If it's the authors name store it
            if (!xmlStrcmp(temp->name, (xmlChar *) "name")) {
                author = malloc(strlen((char *) tempKey));
                strcpy(author, (char *) tempKey);
                
                break;
            }
        }
        
        // Advance to next subtag
        temp = temp->next;	
    }
    
    // If we ever found it, free the key
    if (tempKey)
        xmlFree(tempKey);
        
    return author;
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
            }
            // Check if it was the published timestamp
            else if (!xmlStrcmp(cur->name, (xmlChar *) "published")) {
                article->published = malloc(strlen((char *) key));
                strcpy(article->published, (char *) key);
            }
            else if (!xmlStrcmp(cur->name, (xmlChar *) "content")) {
                // Because HTML elements are in the story, parse it specially
                article->story = parseContent(doc, cur);
            }
            // If it's the author, descend a level and find name
            else if (!xmlStrcmp(cur->name, (xmlChar *) "author")) {
                article->author = findAuthor(doc, cur);
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
 * Display the menu entry item for an article
 */
void displayMenuArticle(ArticleStruct* article, struct winsize window, int index) {
    
    // If it's already read then change the color back to standard
    if (!article->unread)
        attron(COLOR_PAIR(STANDARD_COLOR));
    else
        attron(COLOR_PAIR(MENU_COLOR));
    
    // Print only as many characters as will fit, and print ellipsis?
    printw("%3d : %.*s", index, window.ws_col - 10, article->title);
    strlen(article->title) < window.ws_col - 6 ? printw("\n") : printw("...\n");
    
    // Restore color
    attron(COLOR_PAIR(STANDARD_COLOR));
}


/**
 * Display article's contents using ncurses library
 */
void displayArticleContent(ArticleStruct* article, struct winsize window) {
    
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
 * Initialize NCurses variables and screen, get window information
 */
struct winsize initializeNcurses() {
    // Create ncurses screen
    initscr();
    start_color();
    
    // Get information about terminal window
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    
    // Set up ncurses color pairs
    init_pair(TITLE_COLOR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(AUTHOR_COLOR, COLOR_GREEN, COLOR_BLACK);
    init_pair(STANDARD_COLOR, COLOR_WHITE, COLOR_BLACK);
    init_pair(MENU_COLOR, COLOR_CYAN, COLOR_BLACK);
    
    return window;
}


/**
 * Display header of article view page
 */
void displayHeader(char* title, struct winsize window) {
    // Set menu color
    attron(COLOR_PAIR(MENU_COLOR));
    clear();
    
    // Display header
    PrintBar(window);
    printw("Prompter v 0.0.1");
    mvprintw(1, window.ws_col - 9 - strlen(title), "Source : %s", title);
    PrintBar(window);
    
    // Reset color
    attroff(COLOR_PAIR(MENU_COLOR));
}


/**
 * Display prompt for information and get user's selection
 */
int getSelection(struct winsize window, int articleCount) {
    int option;
    
    attron(COLOR_PAIR(MENU_COLOR));
    
    move(window.ws_row - 2, 0);
    PrintBar(window);
    
    // Prompt for selection and read it in
    printw("Selection (0 to quit) > ");   
    refresh();
    
    // Read in new selection until we get a valid selection
    scanw("%d", &option);
    while(option < 0 || option > articleCount) {
        mvprintw(window.ws_row - 1, 0, "Invalid selection. New selection (0 to quit) > ");
        scanw("%d", &option);
    }
    
    return option;
}


/**
 * Display list of articles using ncurses library
 */
void displayArticles(ArticleStruct** articles, int articleCount, char* title) {
    
    // Initialize variables
    int option = 1, index = 0;
    
    // Initialize ncurses properties
    struct winsize window = initializeNcurses();
    
    while(1) {
        
        // Display header each time
        displayHeader(title, window);
        
        // If we have valid articles
        if (articles) {
            index = 0;
            
            // While we have an article to print
            // Offset index by one to start count at 1
            while(index < articleCount) {  
                displayMenuArticle(articles[index], window, index + 1);
                index++;
            }
        }
        
        option = getSelection(window, articleCount);
    
        // When selection is 0 exit
        if (option == 0)
            break;
        
        // Display specified article and mark it as read
        displayArticleContent(articles[option-1], window);
        articles[option-1]->unread = 0;
    }
    
    // Clear screen
    clear();
    refresh();
}


/**
 * Display feed
 */
void displayFeed(RSSFeed* feed, ArgumentStruct* args) {
    if (feed->articles && feed->articleCount)
        displayArticles(feed->articles, feed->articleCount, feed->title);
    else
        printf("No articles to display\n");
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
void freeArticles(ArticleStruct ** articles, int articleCount) {

    if (articles) {
        for (int index = 0; index < articleCount; index++)
            freeArticle(articles[index]);

        free(articles);
    }
}


/**
 * Free dynamically allocated feed
 */
void freeFeed(RSSFeed* feed) {
    if (feed) {
        freeArticles(feed->articles, feed->articleCount);
        free(feed->url);
        free(feed);
    }
}


/**
 * Configure default RSSFeed struct
 * Dynamically allocated
 */
RSSFeed* getNewFeed(ArgumentStruct* args) {
    RSSFeed* feed = (RSSFeed *) malloc (sizeof(RSSFeed));
    
    feed->url = (char *) malloc(strlen(args->url) + 1);
    strcpy(feed->url, args->url);
    feed->url[strlen(args->url)] = 0;
    
    feed->articleCount = 0;
    
    return feed;
}


/**
 * Get timestamp as tm struct
 */
struct tm getTimeStamp(xmlChar* tmString) {
    struct tm timeStamp;
    
    // Format string that specifically matches the verge, daring fireball
    // Probably not as generic as I would like, but ah well
    strptime((char *) tmString, "%Y-%m-%dT%X-%z", &timeStamp);
    
    return timeStamp;
}


/**
 * Parse through feed XML using libxml2 library
 * Extensively commented for educational purposes
 */
RSSFeed* parseFeed(ArgumentStruct* args) {
    // Doc -> points to the root XML element
    // Cur -> points to current XML element
    xmlDocPtr doc;
    xmlNodePtr cur;
    
    // Allocated RSS feed struct
    RSSFeed* feed = getNewFeed(args);    
    
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
                // Grab title of feed if we can find it
                if (!xmlStrcmp(cur->name, (xmlChar *) "title")) {
                    xmlChar* key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                    
                    if (key) {
                        feed->title = malloc(strlen((char *) key));
                        strcpy(feed->title, (char *) key);
                        
                        free(key);
                    }
                }
                // Set timestamp
                else if (!xmlStrcmp((cur->name), (xmlChar *) "updated")) {
                    xmlChar* key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
                    if (key)
                        feed->timestamp = getTimeStamp(key);
                }
                // If we find a story, parse it, add it to array
                else if (!xmlStrcmp(cur->name, (xmlChar *) "entry")) {
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
 
    // Copy values to feed structure
    feed->articles = articles;
    feed->articleCount = articleCount;
    
    return feed;
}
