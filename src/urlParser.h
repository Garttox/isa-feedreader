#ifndef URLPARSER_H
#define URLPARSER_H

#pragma once

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