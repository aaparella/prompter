#ifndef PARSE_H
	#define PARSE_H

	#include "argparse.h"
	
	typedef struct {
		char* published;
		char* author;
		char* title;
		char* story;
	} ArticleStruct;
	
	void parseFeed(ArgumentStruct* args);                   // Parse RSS data
	void printArticle(ArticleStruct* article);              // Display contents of article
	
#endif
