#include "feedReader.h"

//openssl
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include <iostream>

//read buffer size
#define BUFFER_SIZE 1024

/*
* print error message on stderr
*/
void printErr(std::string msg, std::string urlStr) {
    if(urlStr.size()) {
        std::cerr << urlStr << ": ";
    }
    std::cerr << msg << std::endl;
}

/*
* free BIO and SSL_CTX
*/
void freeSSL(BIO* bio, SSL_CTX* ctx) {
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

/*
* set certificate file
* returns true if certificate file was set successfully or if certFile is empty
*/
bool setCertFile(SSL_CTX* ctx, std::string certFile) {
    if(certFile.size() != 0) {
        return SSL_CTX_load_verify_locations(ctx, certFile.data(), NULL);
    }
    return true;
}

/*
* set certificate location
* returns true if certification directory was set successfully or if certFolder is empty
*/
bool setCertDirectory(SSL_CTX* ctx, std::string certFolder) {
    if(certFolder.size() != 0) {
        return SSL_CTX_load_verify_locations(ctx, NULL, certFolder.data());
    }
    return true;
} 

/*
* set certFile or/and certAddr if they are set(not empty string)
* if certFile or/and certAddr wasnt set, default certificates path will be set
* returns true if any of certificates was set, else returns false
*/
bool setCertificates(SSL_CTX* ctx, std::string certFile, std::string certFolder) {
    bool certFileLoaded = false;
    bool certDirectoryLoaded = false;

    if(certFile.size() != 0) {
        certFileLoaded = setCertFile(ctx, certFile.data());
    }

    if(certFolder.size() != 0) {
        certDirectoryLoaded = setCertDirectory(ctx, certFolder.data());
    }
    
    if(!certFileLoaded && !certDirectoryLoaded) {
        return SSL_CTX_set_default_verify_paths(ctx);
    }
    return true;
}

/*
* initialization of openssl library
*/
void initSSL() {
    SSL_load_error_strings();
    SSL_library_init();
}

std::string getFeed(URL url, std::string certFile, std::string certAddr) {
    SSL* ssl = NULL;
    BIO* bio = NULL;
    SSL_CTX* ctx = NULL;
    //http request
    std::string request = "GET " + url.resource + " HTTP/1.0\r\nHost: " + url.domain + "\r\nConnection: close\r\n\r\n";
    //http response
    std::string response;
    char readBuffer[BUFFER_SIZE];
    
    initSSL();

    if(url.protocol == Protocol::HTTPS) {
        ctx = SSL_CTX_new(SSLv23_client_method());
        if(ctx == NULL) {
            printErr("Nepodařilo se vytvořit SSL kontext.", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }
        bio = BIO_new_ssl_connect(ctx);
        if(bio == NULL) {
            printErr("Nepodařilo se vytvořit BIO", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }

        BIO_get_ssl(bio, &ssl);
        if(ssl == NULL) {
            printErr("Nepodařilo se získat SSL", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }

        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
        //set certificates
        if(!setCertificates(ctx, certFile, certAddr)) {
            printErr("Nepodařilo se načíst certifikáty.", getUrlString(url));
        }

        //set hostname
        BIO_set_conn_hostname(bio, (url.domain + ":" + url.port).data());

        //set SNI
        SSL_set_tlsext_host_name(ssl, url.domain.data());

        //connect
        if(BIO_do_connect(bio) <= 0) {
            printErr("Nepodařilo se připojit k serveru", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }

        long sslRetCode = SSL_get_verify_result(ssl);
        if(sslRetCode != X509_V_OK) {
            printErr("Nepodařilo se ověřit platnost certifikátu", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }
    } else {
        bio = BIO_new_connect((url.domain + ":" + url.port).data());
        if(bio == NULL) {
            printErr("Nepodařilo se vytvořit BIO", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }
        //connect
        if(BIO_do_connect(bio) <= 0) {
            printErr("Nepodařilo se připojit k serveru", getUrlString(url));
            freeSSL(bio, ctx);
            return "";
        }
    }

    bool requestWriteError = false;
    int charWrittenCount = 0;
    /*while(requestWriteError && request.size() != 0) {
        requestWriteError = BIO_write_ex(bio, request.data(), request.size(), &written);
        if(!requestWriteError) {
            if(BIO_should_retry(bio)) {
                requestWriteError = 1;
            }
        } else {
            request.erase(request.begin(), request.begin() + written);
        }
    }*/
    //write request
    while(!requestWriteError && request.size() != 0) {
        charWrittenCount = BIO_write(bio, request.data(), request.size());
        if(charWrittenCount < 0) {
            requestWriteError = true;
            if(BIO_should_retry(bio)) {
                requestWriteError = false;
            }
        } else {
            //erase already written bytes from request
            request.erase(request.begin(), request.begin() + charWrittenCount);
        }
    }
    if(requestWriteError) {
        printErr("Nepodařilo se poslat požadavek", getUrlString(url));
        freeSSL(bio, ctx);
        return "";
    }

    bool responseReadError = false;
    int charReadCount = 0;
    //read response
    do {
        charReadCount = BIO_read(bio, readBuffer, sizeof(readBuffer));
        if(charReadCount < 0) {
            responseReadError = true;
            if(BIO_should_retry(bio)) {
                responseReadError = false;
            }
        } else if(charReadCount > 0) {
            response.append(readBuffer, charReadCount);
        }
    } while(charReadCount > 0);
    if(responseReadError) {
        printErr("Nepodařilo se přečíst odpověď serveru", getUrlString(url));
        freeSSL(bio, ctx);
        return "";
    }

    freeSSL(bio, ctx);

    //check HTTP status code
    if(response.substr(0, response.find("\n")).find("200 OK") == std::string::npos) {
        printErr("Špatný stavový kód HTTP odpovědi.", getUrlString(url));
        return "";
    }
    //remove header
    response.erase(0, response.find("\r\n\r\n") + 4);
    return response;
}
