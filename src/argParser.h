#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>

struct ArgOptions {
    std::string feedUrl;
    std::string feedfile;
    std::string certfile;
    std::string certaddr;
    bool flagFeedfile = false;
    bool flagFeedUrl = false;
    bool flagCertfile = false;
    bool flagCertaddr = false;
    bool flagLastChanged = false;
    bool flagAuthor = false;
    bool flagUrl = false;
};

/*
* Parse the command line arguments into ArgOptions struct.
* Can change content of argv.
* returns ArgOptions
*/
ArgOptions parseArguments(int argc, char *argv[]);

#endif