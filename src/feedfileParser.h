#ifndef FEEDFILEPARSER_H
#define FEEDFILEPARSER_H

#include <iostream>
#include <vector>
#include "urlParser.h"

std::vector<URL> parseFeedfile(std::string feedfileLoc);
void trimSpaces(std::string &line);
#endif
