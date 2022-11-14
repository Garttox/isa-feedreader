#ifndef FEEDREADER_H
#define FEEDREADER_H

#include <string>
#include "url.h"

/*
* Download feed from url. If protocol is https, then it will try to use certFile and cerAddr,
* if it is not empty string
* returns feed or empty string if error occured
*/
std::string getFeed(URL url, std::string certFile, std::string certAddr);

#endif