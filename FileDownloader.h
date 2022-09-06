
#ifndef CLIENT_1_FILEDOWNLOADER_H
#define CLIENT_1_FILEDOWNLOADER_H

#include <cpr/cpr.h>
#include <memory>
#include <iostream>

//------------------------------------------------------------------------

struct RemoteResource{
    std::string host;
    std::string port;
    std::string user;
    std::string password;
    std::string target;
};

//------------------------------------------------------------------------

class FileDownloader{
private:
    const RemoteResource res_;
    std::shared_ptr<cpr::Response> stuff_received_;

    const cpr::Url createURL() const;
    const std::shared_ptr<cpr::Response> fetchRemoteStuff() const;

    const std::string getFetchedHeader() const;
    const std::string getFetchedPayload() const;
    void printServerCertificate() const;
    const std::string getFileName() const;
    const bool is200OK() const;

public:
    FileDownloader() = delete;
    explicit FileDownloader(const RemoteResource&);


    const bool saveFetchedPayload() const;
    const bool hasSuccess() const;
};

//------------------------------------------------------------------------

#endif //CLIENT_1_FILEDOWNLOADER_H
