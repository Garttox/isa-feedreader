#include "xmlReader.h"

#include <iostream>
#include <vector>

//libxml2
#include <libxml/parser.h>
#include <libxml/tree.h>

// inits libxml2 parser
void initXML() {
    xmlInitParser();
}

// free xml document and cleanup libxml2 parser
void freeXML(xmlDocPtr document) {
    xmlFreeDoc(document);
    xmlCleanupParser();
}

/*
* Determinates if xml document is Atom, Rss or none of them
* returns FeedType, if xml document is not Atom or Rss, FeedType::unknown is returned
*/
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

// compares xmlChar string with std::string
bool compareXmlChar(const xmlChar* xmlStr, std::string str) {
    return std::string((const char*)xmlStr) == str;
}

/*
* returns first child node with name nodeName or NULL if no node was found
*/
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

/*
* returns text content of the node
* empty string if node has no text content or the node doesnt exists
*/
std::vector<xmlNodePtr> findNodesByName(xmlNodePtr node, std::string nodeName) {
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

/*
* returns text content of the node
* empty string if node has no text content or the node doesnt exists
*/
std::string getNodeTextContent(xmlNodePtr node) {
    xmlChar* content = xmlNodeGetContent(node);
    std::string contentString = content == NULL ? "" : (char*)content;
    xmlFree(content);
    return contentString;
}

/*
* returns text content of the node's property or 
* empty string if node has no property propertyName or the node doesnt exists
*/
std::string getXmlPropContent(xmlNodePtr node, std::string propertyName) {
    xmlChar* content = xmlGetProp(node, (const xmlChar*)propertyName.data());
    std::string contentString = content == NULL ? "" : (char*)content;
    xmlFree(content);
    return contentString;
}

// print feed's title
void printFeedTitle(std::string title) {
    std::cout << "*** " << title << " ***" << std::endl;
}
// print item's/entry's title
void printTitle(std::string title) {
    std::cout << title << std::endl;
}
// print item's/entry's url
void printUrl(std::string url) {
    std::cout << "URL: " << url << std::endl;
}
// print item's/entry's author
void printAuthor(std::string author) {
    std::cout << "Autor: " << author << std::endl;
}
// print item's/entry's last changed time
void printLastChanged(std::string lastChanged) {
    std::cout << "Aktualizace: " << lastChanged << std::endl;
}

/*
* Prints out Rss feed,
* displayOptions determinates what should be printed from the entries.
* If xml document does not contain data that displayOptions want to print, then option will be ignored
*/
void readRss(xmlNodePtr rootNode, DisplayOptions displayOptions) {
    bool useEntrySeparator = displayOptions.author || displayOptions.lastChanged || displayOptions.url;
    std::vector<xmlNodePtr> items;
    //find required channel node
    xmlNodePtr channelNode = findFirstNodeByName(rootNode, "channel");
    if(!channelNode) {
        //try to search in root node in case that <channel> node is missing
        channelNode = rootNode;
    }
    items = findNodesByName(channelNode, "item");

    printFeedTitle(getNodeTextContent(findFirstNodeByName(channelNode, "title")));
    bool first = true; //indicate first item/iteration
    std::cout << items.size();
    //iterate through all items
    for(xmlNodePtr itemNode : items) {
        if(useEntrySeparator && !first) {
            std::cout << std::endl;
        }
        printTitle(getNodeTextContent(findFirstNodeByName(itemNode, "title")));
        if(displayOptions.lastChanged) {
            xmlNodePtr changedNode = findFirstNodeByName(itemNode, "pubDate");
            if(changedNode) {
                printLastChanged(getNodeTextContent(changedNode));
            }
        }
        if(displayOptions.url) {
            xmlNodePtr urlNode = findFirstNodeByName(itemNode, "link");
            if(urlNode) {
                printUrl(getNodeTextContent(urlNode));
            }
        }
        if(displayOptions.author) {
            xmlNodePtr authorNode = findFirstNodeByName(itemNode, "author");
            if(authorNode) {
                printAuthor(getNodeTextContent(authorNode));
            }
        }
        first = false;
    }
}

/*
* Prints out Atom feed,
* displayOptions determinates what should be printed from the entries.
* If xml document does not contain data that displayOptions want to print, then option will be ignored
*/
void readAtom(xmlNodePtr rootNode, DisplayOptions displayOptions) {
    bool useEntrySeparator = displayOptions.author || displayOptions.lastChanged || displayOptions.url;
    std::vector<xmlNodePtr> entries = findNodesByName(rootNode, "entry");
    bool first = true; //indicate first item/iteration
    printFeedTitle(getNodeTextContent(findFirstNodeByName(rootNode, "title")));
    //iterate through all entries
    for(xmlNodePtr entryNode : entries) {
        if(useEntrySeparator && !first) {
            std::cout << std::endl;
        }
        printTitle(getNodeTextContent(findFirstNodeByName(entryNode, "title")));

        if(displayOptions.lastChanged) {
            xmlNodePtr changedNode = findFirstNodeByName(entryNode, "updated");
            if(changedNode) {
                printLastChanged(getNodeTextContent(changedNode));
            }
        }
        if(displayOptions.url) {
            xmlNodePtr urlNode = findFirstNodeByName(entryNode, "link");
            if(urlNode) {
                printUrl(getXmlPropContent(urlNode, "href"));
            }
        }
        if(displayOptions.author) {
            xmlNodePtr authorNode = findFirstNodeByName(entryNode, "author");
            if(authorNode) {
                xmlNodePtr authorNameNode = findFirstNodeByName(authorNode, "name");
                xmlNodePtr authorEmailNode = findFirstNodeByName(authorNode, "email");
                //author's name has higher priority when printing
                if(authorNameNode) {
                    printAuthor(getNodeTextContent(authorNameNode));
                } else if(authorEmailNode) {
                    printAuthor(getNodeTextContent(authorEmailNode));
                }
            }
        }
        first = false;
    }
}

/*
* print error message on stderr
*/
void printError(std::string msg, std::string feedName) {
    if(feedName.size()) {
        std::cerr << feedName << ": ";
    }
    std::cerr << msg << std::endl;
}

void readXML(std::string feed, DisplayOptions displayOptions, std::string feedName) {
    initXML();
    xmlChar* xmlCharDoc = xmlCharStrdup(feed.data());
    int parserOptions = XML_PARSE_NOWARNING + XML_PARSE_NOERROR;
    xmlDocPtr document = xmlReadDoc(xmlCharDoc, NULL, NULL, parserOptions);
    if(document == NULL) { //if feed is not valid xml
        xmlFree(xmlCharDoc);
        freeXML(document);
        printError("Stažený dokument není ve formátu XML.", feedName);
        return;
    }

    xmlNodePtr rootNode = xmlDocGetRootElement(document);
    FeedType feedType = getFeedType(document);
    if(feedType == FeedType::atom) {
        readAtom(rootNode, displayOptions);
    } else if(feedType == FeedType::rss){
        readRss(rootNode, displayOptions);
    } else {
        printError("Feed není v podporovaném formátu (Atom nebo RSS).", feedName);
    }

    xmlFree(xmlCharDoc);
    freeXML(document);
}