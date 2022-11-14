#include "argParser.h"

#include <getopt.h>
#include <iostream>

#define optString ":f:c:C:Tau"

//prints help message how to use program
void printHelp() {
    std::cerr << "Použití: feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]" << std::endl;
}

ArgOptions parseArguments(int argc, char *argv[]) {
    opterr = 0; //turn off getops's build-in error messages
    int option;
    bool printHelpMsg = false;
    ArgOptions parsedOptionArgs;
    while((option = getopt_long(argc, argv, optString, NULL, NULL)) != -1) {
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
                std::cerr << "Neznámý parametr -" << static_cast<char>(optopt) << std::endl;
                printHelpMsg = true;
                break;
            case ':':
                std::cerr << "Chybějící argument pro -" << static_cast<char>(optopt) << std::endl;
                printHelpMsg = true;
                break;
            default:
                break;
        }
    }
    //if feedfile option is not set, get URL argument
    if(optind < argc) {
        parsedOptionArgs.feedUrl = argv[optind];
        parsedOptionArgs.flagFeedUrl = true;
        optind++;
    }

    //check if feedfile or url option is set
    if(!parsedOptionArgs.flagFeedfile && !parsedOptionArgs.flagFeedUrl) {
        std::cerr << "Nebyl zadán feedfile ani url." << std::endl;
        printHelpMsg = true;
    } else if(parsedOptionArgs.flagFeedfile && parsedOptionArgs.flagFeedUrl) {
        std::cerr << "Byl zadán feedfile i url. Url bude ignorována." << std::endl;
        printHelpMsg = true;
    }

    for(int i = optind; i < argc; i++) {
        std::cerr << "Neplatný parametr/argument: " << argv[i] << " bude ignorován." << std::endl;
        printHelpMsg = true;
    }

    if(printHelpMsg) {
        printHelp();
    }

    return parsedOptionArgs;
}