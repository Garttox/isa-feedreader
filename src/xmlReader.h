#ifndef XMLREADER_H
#define XMLREADER_H

#include <iostream>
#include <vector>
//libxml2
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

enum FeedType {
    atom,
    rss,
    unknown
};

struct DisplayOptions {
    bool lastChanged = false;
    bool author = false;
    bool url = false;
};

void readXML(std::string feed, DisplayOptions displayOptions);

#endif