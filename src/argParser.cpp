#include "argParser.h"

#include <getopt.h>
#include <iostream>

#include "testOut.h"

#define optString ":f:c:CTau"

ArgOptions parseArguments(int argc, char *argv[]) {
    int option;
    bool printHelpMsg = false;
    ArgOptions parsedOptionArgs;
    while((option = getopt(argc, argv, optString)) != -1) {
        switch (option) {
            case 'f':
                parsedOptionArgs.feedfile = optarg;
                parsedOptionArgs.flagFeedfile = true;
                break;
            case 'c':
                parsedOptionArgs.certfile = optarg;
                parsedOptionArgs.flagCertfile = true;
                break;
            case 'C':
                parsedOptionArgs.certaddr = optarg;
                parsedOptionArgs.flagCertaddr = true;
                break;
            case 'T':
                parsedOptionArgs.flagLastChanged = true;
                break;
            case 'a':
                parsedOptionArgs.flagAuthor = true;
                break;
            case 'u':
                parsedOptionArgs.flagUrl = true;
                break;
            case '?':
                std::cout << "Unknown option " << static_cast<char>(optopt) << std::endl;
                printHelpMsg = true;
                break;
            case ':':
                std::cout << "Missing argument for " << static_cast<char>(optopt) << std::endl;
                printHelpMsg = true;
                break;
            default:
                break;
        }
    }

    if(!parsedOptionArgs.flagFeedfile) {
        if(optind < argc) {
            parsedOptionArgs.url = argv[optind];
            optind++;
        }
    }
    for(int i = optind; i < argc; i++) {
        std::cout << "invalid option/argument: " << argv[i] << std::endl;
        printHelpMsg = true;
    }

    if(printHelpMsg) {
        std::cout << "Usage: feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]" << std::endl;
    }

    return parsedOptionArgs;
}