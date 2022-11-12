#include "feedReader.h"

//openssl
#include  "openssl/bio.h"
#include  "openssl/ssl.h"
 #include <openssl/err.h>
//#include  "openssl/err.h"

#include "testOut.h"

#define BUFFER_SIZE 1024

void freeSSL(BIO* bio, SSL_CTX* ctx) {
    BIO_free_all(bio);
    SSL_CTX_free(ctx);
}

bool setCertFile(SSL_CTX* ctx, std::string certFile) {
    if(certFile.size() != 0) {
        return SSL_CTX_load_verify_locations(ctx, certFile.data(), NULL);
    }
    return true;
}

bool setCertDirectory(SSL_CTX* ctx, std::string certFolder) {
    if(certFolder.size() != 0) {
        return SSL_CTX_load_verify_locations(ctx, NULL, certFolder.data());
    }
    return true;
} 

bool setCertificates(SSL_CTX* ctx, std::string certFile, std::string certFolder) {
    bool certFileLoaded = false;
    bool certDirectoryLoaded = false;

    if(certFile.size() != 0) {
        certFileLoaded = SSL_CTX_load_verify_locations(ctx, certFile.data(), NULL);
    }

    if(certFolder.size() != 0) {
        certDirectoryLoaded = SSL_CTX_load_verify_locations(ctx, NULL, certFolder.data());
    }
    
    if(!certFileLoaded && !certDirectoryLoaded) {
        return SSL_CTX_set_default_verify_paths(ctx);
    }
    return true;
}

void initSSL() {
    SSL_load_error_strings();
    SSL_library_init();
}

std::string getFeed(URL url, std::string certFile, std::string certAddr) {
    SSL* ssl = NULL;
    BIO* bio = NULL;
    SSL_CTX* ctx = NULL;
    std::string request = "GET " + url.resource + " HTTP/1.0\r\nHost: " + url.domain + "\r\nConnection: close\r\n\r\n";
    std::string response;
    char readBuffer[BUFFER_SIZE];
    
    initSSL();

    if(url.protocol == Protocol::HTTPS) {
        ctx = SSL_CTX_new(SSLv23_client_method());
        bio = BIO_new_ssl_connect(ctx);
        BIO_get_ssl(bio, &ssl);
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
    
        if(setCertificates(ctx, certFile, certAddr)) {
            //TODO: err
        }

        if(!BIO_set_conn_hostname(bio, (url.domain + ":" + url.port).data())) {
            debugOut("invalid bio HTTPS");
            return "";
        }
            
        //connecting
        if(BIO_do_connect(bio) <= 0) {
            debugOut("invalid connect HTTPS");
            ERR_print_errors_fp(stderr);
            return "";
        }

        if(SSL_get_verify_result(ssl) != X509_V_OK) {
            debugOut("SSL verification err");
            return "";
        }
    } else {
        bio = BIO_new_connect((url.domain + ":" + url.port).data());
        if(bio == NULL) {
            debugOut("invalid bio");
            return "";
        }
        
        if(BIO_do_connect(bio) <= 0) {
            debugOut("invalid connect");
            return "";
            //TODO: retry
        }
    }

    bool requestWriteError = false;
    size_t written;
    while(!requestWriteError || request.size() != 0) {
        requestWriteError = BIO_write_ex(bio, request.data(), request.size(), &written);
        request.erase(request.begin(), request.begin() + written);
    }
    if(requestWriteError) {
        //TODO: write err
    }

    int charReadCount = 0;
    do {
        charReadCount = BIO_read(bio, readBuffer, sizeof(readBuffer));
        if(charReadCount) {
            response.append(readBuffer, charReadCount);
        }
    } while(charReadCount != 0);

    freeSSL(bio, ctx);
    
    

    if(response.substr(0, response.find("\n")).find("200 OK") == std::string::npos) {
        debugOut(response.substr(0, response.find("\n\n")));
        return "";
    }
    response.erase(0, response.find("\r\n\r\n") + 4);
    return response;
}
