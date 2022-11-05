#ifndef FEEDFILEPARSER_H
#define FEEDFILEPARSER_H
#include <iostream>

static bool debug = true;

void debugOut(std::string s) {
    if(debug)
        std::cout << "<DEBUG>" << s << std::endl;
}

#endif
