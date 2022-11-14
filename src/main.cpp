#include <iostream>
#include <string>
#include <fstream>

#include "argParser.h"
#include "feedfileParser.h"
#include "feedReader.h"
#include "xmlReader.h"

/*
* Convert ArgOptions to DisplayOptions
*/
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
        if(!parseFeedfile(argOptions.feedfile, urlsVector)){
            return EXIT_FAILURE; //feedfile cannot be opened
        }
    } else if(argOptions.flagFeedUrl) {
        if(isValidUrl(argOptions.feedUrl)) {
            urlsVector.push_back(parseURL(argOptions.feedUrl));
        }
    } else {
        return EXIT_FAILURE; //feedfile nor url has passed
    }
    
    if(urlsVector.size() == 0) {
        std::cerr << "Nebyly zadány žádné validní URL." << std::endl;
        return EXIT_FAILURE;
    }
    
    DisplayOptions displayOptions = argOptionsToDisplayOptions(argOptions);
    bool first = true; //indicate first url
    for(URL url : urlsVector) {
        if(!first) {
            std::cout << std::endl; //empty line between feeds
        }
        std::string feed = getFeed(url, argOptions.certfile, argOptions.certaddr);
        if(feed.size()) {
            readXML(feed, displayOptions, getUrlString(url));
        }

        first = false;
    } 
    return EXIT_SUCCESS;
}