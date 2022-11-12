#include <iostream>
#include <string>
#include <fstream>

#include "argParser.h"
#include "feedfileParser.h"
#include "feedReader.h"
#include "xmlReader.h"

void writeFile(std::string str, std::string filename) {
    std::ofstream myfile;
    myfile.open (filename);
    myfile << str;
    myfile.close();
}

DisplayOptions argOptionsToDisplayOptions(ArgOptions argOptions) {
    DisplayOptions displayOptions;
    displayOptions.lastChanged = argOptions.flagLastChanged;
    displayOptions.author = argOptions.flagAuthor;
    displayOptions.url = argOptions.flagUrl;
    return displayOptions;
}

int main (int argc, char *argv[]) {
    ArgOptions argOptions;
    std::vector<URL> urlsVector;
    argOptions = parseArguments(argc, argv);
    if(argOptions.flagFeedfile) {
        urlsVector = parseFeedfile("feedfile.txt");
    } else if(isValidUrl(argOptions.url)) {
        urlsVector.push_back(parseURL(argOptions.url));
    }

    if(urlsVector.size() == 0) {
        //TODO: err no valid url(s)
        std::cerr << "No valid url(s) was passed.";
        return EXIT_FAILURE;
    }

    DisplayOptions displayOptions = argOptionsToDisplayOptions(argOptions);
    for(URL url : urlsVector) {
        std::string feed = getFeed(url, argOptions.certfile, argOptions.certaddr);
        writeFile(feed, "testFeed");
        if(feed.size()) {
            readXML(feed, displayOptions);
        }
        //writeFile(feed, "rss.txt");
    } 
    return EXIT_SUCCESS;
}