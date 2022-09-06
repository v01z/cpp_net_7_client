#include "FileDownloader.h"

//------------------------------------------------------------------------

FileDownloader::FileDownloader(const RemoteResource& remote_res):
        res_{ remote_res }, stuff_received_{}
{
    if (res_.target == "/@exit@")
        std::cout << "Trying to stop server..\n";

    stuff_received_ = fetchRemoteStuff();

    printServerCertificate();

    std::string header { getFetchedHeader() };
    if(!header.empty())
        std::cout << "\nGot header from remote host:\n\n"
            << getFetchedHeader() << std::endl;

}

//------------------------------------------------------------------------

const cpr::Url FileDownloader::createURL() const {
    return "https://" + res_.host + ":" + res_.port + res_.target;
}

//------------------------------------------------------------------------

const std::shared_ptr<cpr::Response> FileDownloader::fetchRemoteStuff() const {

    cpr::SslOptions ssl_opts { cpr::Ssl(cpr::ssl::TLSv1_3{}) };
    ssl_opts.verify_peer = false; // Because we use self-signed cert on server.
    ssl_opts.verify_host = false; // If 'true', then '127.0.0.1' != 'localhost'.

    return std::make_shared<cpr::Response> ( cpr::Get(createURL(), ssl_opts,
        cpr::Authentication{res_.user,
    res_.password, cpr::AuthMode::BASIC}));
}

//------------------------------------------------------------------------

const std::string FileDownloader::getFetchedHeader() const {
    if(hasSuccess())
        return stuff_received_->raw_header;

    return {};
}

//------------------------------------------------------------------------

const std::string FileDownloader::getFetchedPayload() const {

    if(hasSuccess())
        return stuff_received_->text;

    return {};
}

//------------------------------------------------------------------------

void FileDownloader::printServerCertificate() const {
    if(!hasSuccess())
        return;

    std::cout << "Connection is secure and server \'"
        << res_.host << "\' uses certificate:\n\n";

    std::vector<cpr::CertInfo> cert_info = stuff_received_->GetCertInfos();

    for (const auto& elem: cert_info) {
        for (const std::string& entry : elem) {
            std::cout << entry << std::endl;
        }
    }
}

//------------------------------------------------------------------------

const bool FileDownloader::saveFetchedPayload() const {
    if(!hasSuccess())
        return false;

    if(!is200OK())
        return false;

    std::string file_name { getFileName() };
    std::ofstream file(file_name ,std::ios::binary);
    if(!file.is_open())
        return false;

    file.write(getFetchedPayload().c_str(),
    sizeof(char)*getFetchedPayload().size());

    if(std::stol(stuff_received_->header["Content-Length"]) == file.tellp())
        std::cout << "File \'" << file_name
            << "\' has been downloaded and saved successfully.\n";
    else
        std::cout << "There was something wrong during getting file "
            << file_name << '.' << std::endl;

    return true;
}

//------------------------------------------------------------------------

const bool FileDownloader::hasSuccess() const {
    return stuff_received_ != nullptr;
}

//------------------------------------------------------------------------

const bool FileDownloader::is200OK() const {
    if(!hasSuccess())
        return false;

    long status_code = stuff_received_->status_code;
    if(status_code == 200) // 200 OK
        return true;

    if(status_code == 0) 
    {
        std::cout << "Server \'" << res_.host << "\' is down.\n";
        return false;
    }

    std::cout << "Server \'" << res_.host << "\' responded with "
        << status_code << " code.\n";

    return false;
}

//------------------------------------------------------------------------

const std::string FileDownloader::getFileName()const{
        return res_.target.substr(res_.target.find_last_of("/\\") + 1);
}

//------------------------------------------------------------------------
