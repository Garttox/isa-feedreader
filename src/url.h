#ifndef URL_H
#define URL_H

#include <string>

enum Protocol {
    HTTP,
    HTTPS
};

struct URL {
    Protocol protocol;
    std::string port;
    std::string domain;
    std::string resource;
};

bool isValidUrl(std::string urlString);
URL parseURL(std::string urlString);

#endif