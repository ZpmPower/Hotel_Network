#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include "RRManager.h"
#include <utility>

class MessageManager
{
public:
    MessageManager(const std::string& address, const std::string& port);

    void start();    
    void setOnErrorCB(const std::function<void (ClientError error)> &onError);
    void setOnReadCB(const std::function<void (const network::ResponseContext&)> &onRead);
    void execute(ByteBufferPtr buff);
    void execute(const std::string& buff);
    bool userAuth(const std::string& login, const std::string& password);
    bool createUser(const std::string& login, const std::string& password, const std::string &fname, const std::string &sname, const std::string &lname,
                    const std::string &phone, const std::string &passport);
    bool createEmployee(const std::string& login, const std::string& password, const std::string &fname, const std::string &sname,
                        const std::string &lname, const std::string &phone, int64_t salary, int32_t position, int32_t hotelid);
    void getGuests();
    void getEmployees();
    void getHotels();
    void getRooms();
private:

    std::shared_ptr<RRMananger> clientChatPtr_;
    asio::ssl::context context_;
};

#endif // MESSAGEMANAGER_H
