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
    bool createGuest(const std::string& login, const std::string& password, const std::string &fname, const std::string &sname, const std::string &lname,
                    const std::string &phone, const std::string &passport, uint32_t role);
    bool createEmployee(const std::string& login, const std::string& password, const std::string &fname, const std::string &sname,
                        const std::string &lname, const std::string &phone, int64_t salary, int32_t position, int32_t hotelid, uint32_t role);
    bool editEmployee(uint32_t employeeId, const std::string &fname, const std::string &sname,
                      const std::string &lname, const std::string &phone, int64_t salary, const std::string &position, int32_t hotelid);
    void getGuests();
    void getEmployees();
    void getHotelEmployees(uint32_t hotelid);
    void getHotels();
    void getRooms();

    void countHotelRooms(uint32_t hotelid);
    void countHotelEmployees(uint32_t hotelid);
    void countHotelOrders(uint32_t hotelid);
    void avgResidenceTime(uint32_t hotelid);
    void avgRoomRating(uint32_t hotelid);

    void getHotelRooms(uint32_t hotelid);
    void getHotelOrders(uint32_t hotelid);
    void deleteEmployee(uint32_t employeeId);
    void deleteRoom(uint32_t roomId);
    network::SessionInfo& getSession() {return sessionInfo_;}
    void setSession(const network::SessionInfo& session) { sessionInfo_ = session;}
    void getRoomTypes();
    void getHotelTypes();
    void getCurrentGuests(uint32_t hotel_id);
    void getVacantRooms(const std::string& datebegin,const std::string& dateend, uint32_t capacity,uint32_t startPrice, uint32_t endPrice,
                        uint32_t startRating, uint32_t endRating, const std::string& room_type, uint32_t hotel_id);
    void makeOrder(const std::string& datebegin,const std::string& dateend,uint32_t idroom,uint32_t idemployee, uint32_t idguest);
    void addRoom(uint32_t places,uint32_t price,uint32_t rating, bool status, uint32_t floor,const std::string& type, uint32_t hotel_id);
    void editRoom(uint32_t id,uint32_t places,uint32_t price,uint32_t rating, bool status, uint32_t floor,const std::string& type, uint32_t hotel_id);
private:

    std::shared_ptr<RRMananger> clientChatPtr_;
    asio::ssl::context context_;
    network::SessionInfo sessionInfo_;
};

#endif // MESSAGEMANAGER_H
