#include <stdio.h>
#include <stdlib.h>
#include <libxml/xmlreader.h>
#include "parse.h"


/**
 * Assign default argument values
 */
ArgumentStruct* getDefaultArguments() {
	ArgumentStruct* defaults = malloc(sizeof(ArgumentStruct));
	
	defaults->url = "http://www.theverge.com/rss/frontpage";
	defaults->stdout = 0;
	
	return defaults;
}

/**
 * Parse command line options
 * Return a configured ArgumentStruct* on success
 * Return NULL if arguments cannot be successfuly parsed
 */
ArgumentStruct* parseArguments(int argc, char* argv[]) {
	// Initially assume all arguments to be default
	ArgumentStruct* args;
	args = getDefaultArguments();
	
	// Read through options provided
	// Yeah okay whatever I'll get to this eventually
	
	return args;	
}

/**
 * Parse individual story
 * Internal use only, not in header
 */
void parseStory(xmlDocPtr doc, xmlNodePtr storyRoot) {
	// Extract basic information from each story
	xmlNodePtr cur = storyRoot->xmlChildrenNode;
	xmlChar* key;
	
	// While we have subelements to go through
	while(cur) {
		// Example : If we find the article title
		if (!xmlStrcmp(cur->name, (xmlChar *) "title")) {
			key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			printf("article title : %s\n", key);
			xmlFree(key);
		}
		
		cur = cur->next;
	}
}

/**
 * Parse through feed XML using libxml2 library
 * Extensively commented for educational purposes
 */
void parseFeed() {
	// Doc -> points to the root XML element
	// Cur -> points to current XML element
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	// Parse file
	doc = xmlReadFile("/Library/Caches/prompter.tmp", NULL, XML_PARSE_RECOVER);
	
	// doc is null if file could not be parsed
	if (doc) {
		cur = xmlDocGetRootElement(doc);
		
		// Because we got root, descend one level
		if (cur)
			cur = cur->xmlChildrenNode;
			
		while(cur) {	
			if (!xmlStrcmp(cur->name, (xmlChar *) "entry"))
				parseStory(doc, cur);
				
			cur = cur->next;	
		}
	}
}