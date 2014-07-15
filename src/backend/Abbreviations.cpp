#include "backend/Abbreviations.h"
#include <iostream>
#include <iomanip>

void Abbreviations::newLine(int num){
	for (int i=0; i<num; i++)
		std::cout << std::endl;			// making an abbreviaton method for new lines
}

/*- -------------------------------------------------------------- -*/
void Abbreviations::tabLeft(int tab){
	for (int i=0; i<tab; i++)			// making an abbreviation method for tab spacing
		std::cout << "\t";
}

/*- -------------------------------------------------------------- -*/
void Abbreviations::set_precision(int precision){
	std::cout << std::setprecision(precision) << std::fixed;	// making an abbreviation method for cout precision
}

/*- -------------------------------------------------------------- -*/
void Abbreviations::skip_page(){								
	std::cout << std::string(100, '\n');						// skiping a whole console page
}
/*- -------------------------------------------------------------- -*/
