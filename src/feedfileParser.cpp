#include "feedfileParser.h"

#include <fstream>
#include <string>
#include <regex>
#include "urlParser.h"

//trim blank spaces from left and right
void trimSpaces(std::string &line) {
    std::string::iterator strIterator = line.begin();
    //iterate until first character that is not space
    while (*strIterator == ' ' && strIterator != line.end()) {
        strIterator++;
    }
    line.erase(line.begin(), strIterator - 1);
    
    int controlCharacters = 0;
    strIterator = line.end();
    //skip newline and EOF 
    while(*strIterator != ' ' && strIterator != line.begin()) {
        //exclude '\0', erase method doesnt count '\0'
        if(*strIterator != '\0') {
            controlCharacters++;
        }
        if(*strIterator == EOF) {
            std::cout << "EOF";
        }
        strIterator--;
    }

    while(*strIterator == ' ' && strIterator != line.begin()) {
        strIterator--;
    }
    
    line.erase(strIterator, line.end() - controlCharacters);
}

std::vector<URL> parseFeedfile(std::string feedfileLoc) {
    std::vector<URL> urlsVector;
    std::ifstream feedfile;
    //open file
    feedfile.open(feedfileLoc, std::ios::out);
    if(!feedfile.is_open()) {
        throw ("File " + feedfileLoc + " could not be opened."); //feedfile could not be opened or location is invalid
    }
    std::string line;
    while(std::getline(feedfile, line)) {
        trimSpaces(line);
        if(line.size() != 0) {
            //std::cout << line << std::endl;
            if(isValidUrl(line)) { // TODO: regex
                //std::cout << "valid" << std::endl;
                urlsVector.push_back(parseURL(line));
                
            } else if(line[0] != '#') {
                //TODO: unknown line
            }
        }
    }

    feedfile.close();

    return urlsVector;
}