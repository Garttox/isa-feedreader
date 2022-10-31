#include "argParser.h"

#include <getopt.h>
#include <iostream>

#define optString ":f:c:CTau"

ArgOptions parseArguments(int argc, char *argv[]) {
    int option;
    ArgOptions parsedOptionArgs;
    while((option = getopt(argc, argv, optString)) != -1) {
        switch (option) {
            case 'f':
                std::cout << "-f " << optarg << std::endl;
                parsedOptionArgs.feedfile = optarg;
                parsedOptionArgs.flagFeedfile = true;
                break;
            case 'c':
                std::cout << "-c " << optarg << std::endl;
                break;
            case 'C':
                std::cout << "-C " << optarg << std::endl;
                break;
            case 'T':
                std::cout << "-T" << std::endl;
                break;
            case 'a':
                std::cout << "-a" << std::endl;
                break;
            case 'u':
                std::cout << "-u" << std::endl;
                break;
            case '?':
                std::cout << "Unknown option " << optopt << std::endl;
                break;
            case ':':
                std::cout << "Missing arg for " << optopt << std::endl;
                break;
            default:
                break;
        }
    }
    
    return parsedOptionArgs;
}