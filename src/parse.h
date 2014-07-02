#ifndef PARSE_H
	#define PARSE_H

	typedef struct {
		char* url;  // RSS feed URL
		char* storagePath;
		int stdout; // Write result to stdout or not
	} ArgumentStruct;
	
	typedef struct {
		char* published;
		char* author;
		char* title;
		char* story;
	} ArticleStruct;
	
	ArgumentStruct* getDefaultArguments();                  // Initialize default arguments
	ArgumentStruct* parseArguments(int argc, char* argv[]); // Parse command line arguments
	void parseFeed();                                       // Parse RSS data
	void printArticle(ArticleStruct* article);              // Display contents of article
	
#endif
