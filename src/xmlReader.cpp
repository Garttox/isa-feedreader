#include "xmlReader.h"

void initXML() {
    xmlInitParser();
}

void freeXML(xmlDocPtr document) {
    xmlFreeDoc(document);
    xmlCleanupParser();
}

FeedType getFeedType(xmlDocPtr document) {
    xmlNodePtr rootNode = xmlDocGetRootElement(document);
    if(rootNode->name) {
        if(std::string((const char*)rootNode->name) == "feed") {
            return FeedType::atom;
        } else if(std::string((const char*)rootNode->name) == "rss") {
            return FeedType::rss;
        }
    }
    return FeedType::unknown;
}

void addNsToXPath(std::string &xpath, std::string ns) {
    size_t index = xpath.find('/');
    while(index != std::string::npos) {
        if(index != xpath.size()-1 && xpath[index + 1] != '/') {
            xpath.insert(index + 1, ns + ":");
        }
        index = xpath.find('/', index+1);
    }
}

std::string findNodeByNameOld(xmlDocPtr document, std::string nodeXPath) {
    xmlXPathContextPtr xpathContext = xmlXPathNewContext(document);
    xmlXPathRegisterNs(xpathContext, (const xmlChar*)"Atom", (const xmlChar*)"http://www.w3.org/2005/Atom");
    
    xmlXPathObjectPtr xmlXPathObj = xmlXPathEval((const xmlChar*)nodeXPath.data(), xpathContext);
    xmlXPathFreeContext(xpathContext);
    //std::cout << xmlXPathObj->nodesetval->nodeNr << std::endl;
    int size = xmlXPathObj->nodesetval->nodeNr;
    for(int i = 0; i < size; i++) {
        //std::cout << xmlXPathObj->nodesetval->nodeTab[i]->last->content << std::endl;
    }
    
    std::string ret = std::string((const char*)xmlXPathObj->nodesetval->nodeTab[0]->last->content);
    xmlXPathFreeObject(xmlXPathObj);
    return ret;
}

bool compareXmlChar(const xmlChar* xmlStr, std::string str) {
    return std::string((const char*)xmlStr) == str;
}

xmlNodePtr findFirstNodeByName(xmlNodePtr node, std::string nodeName) {
    if(node->children == NULL) {
        return NULL;
    }
    xmlNodePtr nodeChild = node->children;
    while(nodeChild != NULL) {
        if(compareXmlChar(nodeChild->name, nodeName)) {
            return nodeChild;
        }
        nodeChild = nodeChild->next;
    }
    return NULL;
}

//xmlXPathObjectPtr needs to be freed
std::vector<xmlNodePtr> findNodesByName(xmlNodePtr node, std::string nodeName) {
    /*xmlXPathObjectPtr xmlXPathObj = xmlXPathEval((const xmlChar*)nodeXPath.data(), xpathContext);
    //if xmlXPathEval is unsuccessful return NULL
    if(xmlXPathObj && xmlXPathObj->nodesetval->nodeNr == 0) {
        return NULL;
    }
    return xmlXPathObj;*/

    std::vector<xmlNodePtr> resultNodes;
    if(node->children == NULL) {
        return resultNodes;
    }
    xmlNodePtr nodeChild = node->children;
    while(nodeChild != NULL) {
        if(compareXmlChar(nodeChild->name, nodeName)) {
            resultNodes.push_back(nodeChild);
        }
        nodeChild = nodeChild->next;
    }
    return resultNodes;
}

std::string getNodeTextContent(xmlNodePtr node) {

    return std::string((const char*)node->last->content);
}

void testOutAtomLine(xmlXPathContextPtr xpathContext, xmlNodePtr node, std::string a) {
    xmlXPathObjectPtr xmlXPathObj;
    xmlXPathObj = xmlXPathNodeEval(node, (const xmlChar*)a.data(), xpathContext);
    if(xmlXPathObj != NULL)
        std::cout << "*** " << xmlXPathObj->nodesetval->nodeTab[0]->last->content << " ***" << std::endl;
}

void testOutAtom(xmlDocPtr document, DisplayOptions displayOptions) {
    /*xmlXPathContextPtr xpathContext = xmlXPathNewContext(document);
    xmlXPathRegisterNs(xpathContext, (const xmlChar*)"a", (const xmlChar*)"http://www.w3.org/2005/Atom");
    xmlXPathObjectPtr xmlXPathObj;
    xmlXPathObjectPtr xmlXPathObjEntry = xmlXPathEval((const xmlChar*)"//a:feed/a:entry", xpathContext);
    //std::cout << xmlXPathObjEntry->nodesetval->nodeNr << std::endl;
    std::cout << (xmlXPathObjEntry->nodesetval == NULL ? "true" : "false") << "ATOM";
    std::cout << xmlXPathObjEntry->nodesetval->nodeNr;
    return;
    int size = xmlXPathObjEntry->nodesetval->nodeNr;
    for(int i = 0; i < size; i++) {
        //std::cout << xmlXPathObjEntry->nodesetval->nodeTab[i]->last->content << std::endl;
        xmlXPathObj = xmlXPathNodeEval(xmlXPathObjEntry->nodesetval->nodeTab[i], (const xmlChar*)"Atom:title", xpathContext);
        if(xmlXPathObj->nodesetval != NULL)
            std::cout << xmlXPathObj->nodesetval->nodeTab[0]->last->content << std::endl;
        xmlXPathFreeObject(xmlXPathObj);
    }
    xmlXPathFreeObject(xmlXPathObjEntry);
    xmlXPathFreeContext(xpathContext);*/
    
    
    /*std::vector<xmlNodePtr> resultNodes;
    xmlNodePtr rootNode = xmlDocGetRootElement(document);
    xmlNodePtr node = rootNode->children;
    std::cout << rootNode->children->name << std::endl;
    while(node != NULL) {
        if(std::string((const char*)node->name) == "entry") {
            xmlNodePtr entryNode = node->children;
            while(entryNode != NULL) {
                if(std::string((const char*)entryNode->name) == "title") {
                    std::cout << getNodeTextContent(entryNode) << std::endl;
                } else if(std::string((const char*)entryNode->name) == "updated") {
                    std::cout << "Aktualizace: " << getNodeTextContent(entryNode) << std::endl;
                } else if(std::string((const char*)entryNode->name) == "id") {
                    std::cout << "URL: " << getNodeTextContent(entryNode) << std::endl;
                } else if(std::string((const char*)entryNode->name) == "updated") {

                    std::cout << "Autor: " << getNodeTextContent(entryNode) << std::endl;
                }
                entryNode = entryNode->next;
            }
            std::cout << std::endl;
        }
        
        node = node->next;
    }
    return resultNodes;*/
    bool useEntrySeparator = displayOptions.author || displayOptions.lastChanged || displayOptions.url;
    xmlNodePtr rootNode = xmlDocGetRootElement(document);
    std::cout << "*** " << getNodeTextContent(findFirstNodeByName(rootNode, "title")) << " ***" << std::endl;
    std::vector<xmlNodePtr> entries = findNodesByName(rootNode, "entry");
    for(xmlNodePtr entryNode : entries) {
        std::cout << getNodeTextContent(findFirstNodeByName(entryNode, "title")) << std::endl;
        if(displayOptions.lastChanged) {
            xmlNodePtr changedNode = findFirstNodeByName(entryNode, "updated");
            if(changedNode) {
                std::cout << "Aktualizace: " << getNodeTextContent(changedNode) << std::endl;
            }
        }
        if(displayOptions.url) {
            xmlNodePtr urlNode = findFirstNodeByName(entryNode, "link");
            if(urlNode) {
                xmlAttrPtr hrefNode = urlNode->properties;
                while(!compareXmlChar(hrefNode->name, "href")) {
                    hrefNode = hrefNode->next;
                }
                std::cout << "URL: " << hrefNode->last->content << std::endl;
            } else {
                urlNode = findFirstNodeByName(entryNode, "id");
                if(urlNode) {
                    std::cout << "URL: " << getNodeTextContent(urlNode) << std::endl;
                }
            }
        }
        if(displayOptions.author) {
            xmlNodePtr authorNode = findFirstNodeByName(entryNode, "author");
            if(authorNode) {
                xmlNodePtr authorNameNode = findFirstNodeByName(authorNode, "name");
                if(authorNameNode) {
                    std::cout << "Autor: " << getNodeTextContent(authorNameNode) << std::endl;
                } else {
                    xmlNodePtr authorEmailNode = findFirstNodeByName(authorNode, "email");
                    if(authorEmailNode) {
                        std::cout << "Autor: " << getNodeTextContent(authorEmailNode) << std::endl;
                    }
                }
            }
        }
        if(useEntrySeparator) {
            std::cout << std::endl;
        }
    }
}

void testOutRss(xmlDocPtr document, DisplayOptions displayOptions) {
    bool useEntrySeparator = displayOptions.author || displayOptions.lastChanged || displayOptions.url;
    xmlNodePtr rootNode = xmlDocGetRootElement(document);
    xmlNodePtr channelNode = findFirstNodeByName(rootNode, "channel");
    std::cout << "*** " << getNodeTextContent(findFirstNodeByName(channelNode, "title")) << " ***" << std::endl;
    std::vector<xmlNodePtr> items = findNodesByName(channelNode, "item");
    for(xmlNodePtr itemNode : items) {
        std::cout << getNodeTextContent(findFirstNodeByName(itemNode, "title")) << std::endl;
        if(displayOptions.lastChanged) {
            xmlNodePtr changedNode = findFirstNodeByName(itemNode, "pubDate");
            if(changedNode) {
                std::cout << "Aktualizace: " << getNodeTextContent(changedNode) << std::endl;
            }
        }
        if(displayOptions.url) {
            xmlNodePtr urlNode = findFirstNodeByName(itemNode, "link");
            if(urlNode) {
                std::cout << "URL: " << getNodeTextContent(urlNode) << std::endl;
            }
        }
        if(displayOptions.author) {
            xmlNodePtr authorNode = findFirstNodeByName(itemNode, "creator");
            if(authorNode) {
                std::cout << "Autor: " << getNodeTextContent(authorNode) << std::endl;
            }
        }
        if(useEntrySeparator) {
            std::cout << std::endl;
        }
    }
}

void readXML(std::string feed, DisplayOptions displayOptions) {
    initXML();
    xmlChar* xmlCharDoc = xmlCharStrdup(feed.data());
    xmlDocPtr document = xmlParseDoc(xmlCharDoc);

    FeedType feedType = getFeedType(document);
    if(feedType == FeedType::atom) {
        testOutAtom(document, displayOptions);
    } else {
        testOutRss(document, displayOptions);
    }

    xmlFree(xmlCharDoc);
    freeXML(document);
}