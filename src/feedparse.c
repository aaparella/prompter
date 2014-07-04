#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlreader.h>
#include <sys/ioctl.h>
#include "feedparse.h"
#include "argparse.h"

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"



/** 
 * Parse conents of a story
 * Have to handle the special HTML tags specially
 */
char* parseContent(xmlDocPtr doc, xmlNodePtr contentRoot) {
	xmlChar* rawStory = xmlNodeListGetString(doc, contentRoot->xmlChildrenNode, 1);
	return (char *) rawStory;
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
 * Display article's contents
 */
void displayArticle(ArticleStruct* article) {
	
	// If non-null, print each non-null field
	if (article) {
		if (article->title)
			printf("%s%s\n", KGRN, article->title);
		if (article->published)
			printf("%s%s%s\n", KYEL, article->published, KNRM);
		if (article->author)
			printf("%s\n", article->author);	
		if (article->story)
			printf("%s\n", article->story);
	}
	
	printf("\n");
}

/**
 * Parse through feed XML using libxml2 library
 * Extensively commented for educational purposes
 */
void parseFeed(ArgumentStruct* args) {
	// Doc -> points to the root XML element
	// Cur -> points to current XML element
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	// Parse file
	doc = xmlReadFile("/Library/Caches/prompter.tmp", NULL, XML_PARSE_RECOVER);
	
	// doc is null if file could not be parsed
	if (doc) {
		// Get the root of the parse tree
		cur = xmlDocGetRootElement(doc);
		
		// Because we got tree root, descend one level
		if (cur)
			cur = cur->xmlChildrenNode;
			
		while(cur) {	
			// If we find a story, parse it, display it, and free the object
			if (!xmlStrcmp(cur->name, (xmlChar *) "entry")) {
				ArticleStruct* article = parseStory(doc, cur);
				displayArticle(article);
				freeArticle(article);
			}
			
			cur = cur->next;	
		}
	}
}