#ifndef PARSE_H
	#define PARSE_H

	typedef struct {
		char* url;          // RSS feed URL
		char* storagePath;  // Where to write temp files
		int stdout;         // Write result to stdout or not
		int articleCount;   // Number of articles to display
	} ArgumentStruct;
	
	typedef struct {
		char* published;
		char* author;
		char* title;
		char* story;
	} ArticleStruct;
	
	void displayUsage();                                    // Display usage guidelines and quit
	
	ArgumentStruct* getDefaultArguments();                  // Initialize default arguments
	ArgumentStruct* parseArguments(int argc, char* argv[]); // Parse command line arguments
	void parseFeed(ArgumentStruct* args);                   // Parse RSS data
	void printArticle(ArticleStruct* article);              // Display contents of article
	
#endif
