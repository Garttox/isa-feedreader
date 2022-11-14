#ifndef FEEDFILEPARSER_H
#define FEEDFILEPARSER_H

#include <iostream>
#include <vector>

#include "url.h"
/*
* parse feedFile, ignores leading and trailing spaces
* push all valid urls into urlsVector
* returns true if parsing was successful or false if file could not be opened
*/
bool parseFeedfile(std::string feedfileLoc, std::vector<URL> &urlsVector);

#endif
