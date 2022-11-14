#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>

//type of feed xml
enum FeedType {
    atom,
    rss,
    unknown
};

//options for determinating what should be displayed from feed
struct DisplayOptions {
    bool lastChanged = false;
    bool author = false;
    bool url = false;
};

/*
* Supports only Atom and RSS 2.0 formats.
* Writes feed title, titles of etries and other data if displayOptions are set, into stdout.
* If the data requested in displayOptions is not available in the feed, it will not be displayed.
*/
void readXML(std::string feed, DisplayOptions displayOptions, std::string feedName);

#endif