#ifndef ABBREVIATIONS_H
#define ABBREVIATIONS_H

class Abbreviations{
	public:
		void newLine(int num);				// print 'num' new lines
		void tabLeft(int tab);				// print 'tab' tabs (\t)
		void set_precision(int precision);	// sets the precission for the following cout 
		void skip_page();					// skips a console page
};

#endif