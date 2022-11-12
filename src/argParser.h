#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <string>

struct ArgOptions {
    std::string url;
    std::string feedfile;
    std::string certfile;
    std::string certaddr;
    bool flagFeedfile = false;
    bool flagCertfile = false;
    bool flagCertaddr = false;
    bool flagLastChanged = false;
    bool flagAuthor = false;
    bool flagUrl = false;
};

ArgOptions parseArguments(int argc, char *argv[]);

#endif