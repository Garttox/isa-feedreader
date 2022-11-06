#include "url.h"

#include <regex>
#include <iostream>

bool isValidUrl(std::string urlString) {
    return std::regex_match(urlString, std::regex("^(http|https)://.+$"));
}

URL parseURL(std::string urlString) {
    URL url;
    int protocolEndInd = urlString.find("://");
    url.protocol = urlString.substr(0, protocolEndInd) == "http" ? Protocol::HTTP : Protocol::HTTPS;
    std::cout << "protokol: " << urlString.substr(0, protocolEndInd) << std::endl;
    urlString.erase(0, protocolEndInd + 3);

    bool hasPort = false;

    if(urlString.find(":") != std::string::npos) {
        hasPort = true;
    }


    int domainEndInd = urlString.find(hasPort ? ":" : "/");
    url.domain = urlString.substr(0, domainEndInd);
    urlString.erase(0, domainEndInd + (hasPort ? 1 : 0));

    if(hasPort) {
        int portEndInd = urlString.find("/");
        url.port = urlString.substr(0, portEndInd);
        urlString.erase(0, portEndInd);
        std::cout << "port: " << url.port << std::endl;
    }

    std::cout << "domain: " << url.domain << std::endl;
    
    url.resource = urlString;
    std::cout << "resource: " << url.resource << std::endl;

    return url;
}
