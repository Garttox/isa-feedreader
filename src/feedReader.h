#ifndef FEEDREADER_H
#define FEEDREADER_H

#include <string>
#include "url.h"

std::string getFeed(URL url, std::string certFile, std::string certAddr);

#endif