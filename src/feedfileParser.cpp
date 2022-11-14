#include "feedfileParser.h"

#include <fstream>
#include <string>
#include <regex>

//trim leading spaces and tabs
void trimSpacesLeft(std::string &line) {
    auto nonSpaceCharIndex = line.find_first_not_of(" \t");
    line.erase(0, nonSpaceCharIndex);
}

//trim trailing spaces and tabs
void trimSpacesRight(std::string &line) {
    auto nonSpaceCharIndex = line.find_last_not_of(" \t");
    if(nonSpaceCharIndex != std::string::npos)
        line.erase(nonSpaceCharIndex + 1);
}

bool parseFeedfile(std::string feedfileLoc, std::vector<URL> &urlsVector) {
    std::ifstream feedfile;
    //open file
    feedfile.open(feedfileLoc, std::ios::out);
    if(!feedfile.is_open()) {
        //feedfile could not be opened or location is invalid
        std::cerr << "Soubor \"" << feedfileLoc << "\" se nepodařilo otevřít, nebo soubor neexistuje." << std::endl;
        return false;
    }
    std::string line;
    int lineNumber = 1; //number of current line, used to print where invalid URL was found
    while(std::getline(feedfile, line)) {
        trimSpacesLeft(line);
        if(line.size() != 0 && line[0] != '#') {
            trimSpacesRight(line);
            if(isValidUrl(line)) {
                urlsVector.push_back(parseURL(line));
            } else {
                std::cerr << "Feedfile:" << lineNumber << ": \"" << line << "\" není validní url, řádek bude ignorován." << std::endl;
            }
        }
        lineNumber++;
    }

    feedfile.close();

    return true;
}