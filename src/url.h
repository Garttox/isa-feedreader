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

/*
* Check if urlString is valid URL,
* only allowed protocols are http and https
*/
bool isValidUrl(std::string urlString);

/*
* parse urlString into URL struct
* urlString needs to be validated first (isValidUrl)
* returns URL struct
*/
URL parseURL(std::string urlString);

/*
* builds up url from URL struc
* returns string representation of URL
*/
std::string getUrlString(URL url);

#endif