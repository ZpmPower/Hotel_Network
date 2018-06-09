#ifndef HOTELPOSTGRESMANAGER_H
#define HOTELPOSTGRESMANAGER_H

#include "DBHelper.h"
#include "define.h"

struct GuestInfo;
struct EmployeeInfo;
struct HotelInfo;
struct RoomInfo;
struct OrderInfo;
struct HotelType;
struct GuestOrder;

class HotelPostgresManager
{
public:
    static db_connection_ptr checkConnection(uint32_t role);
    static ResponseCode createGuest(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                    const std::string phone, const std::string passport);
    static ResponseCode createEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                    const std::string phone, int64_t salary, int32_t position, int32_t hotelid);
    static ResponseCode getEmployeeInfo(int64_t user_id,EmployeeInfo &eInfo);
    static ResponseCode getGuestInfo(int64_t user_id, GuestInfo &gInfo);
    static ResponseCode getGuests(std::vector<GuestInfo>& guests);
    static ResponseCode getCurrentGuests(std::vector<GuestOrder>& guests, uint32_t hotel_id);
    static ResponseCode getEmployees(std::vector<EmployeeInfo>& employees);
    static ResponseCode getHotelEmployees(std::vector<EmployeeInfo>& employees, uint32_t hotelid, uint32_t role);
    static ResponseCode getHotels(std::vector<HotelInfo>& hotels);
    static ResponseCode getRooms(std::vector<RoomInfo>& rooms);
    static ResponseCode countHotelRooms(uint32_t &countRooms, uint32_t hotelid, uint32_t role);
    static ResponseCode countHotelEmployees(uint32_t &countEmployees, uint32_t hotelid, uint32_t role);
    static ResponseCode countHotelOrders(uint32_t &countOrders, uint32_t hotelid, uint32_t role);
    static ResponseCode avgResidenceTime(double &avgTime, uint32_t hotelid, uint32_t role);
    static ResponseCode avgRoomRating(double &avgRating, uint32_t hotelid, uint32_t role);
    static ResponseCode getHotelRooms(std::vector<RoomInfo>& rooms, uint32_t hotelid, uint32_t role);
    static ResponseCode getVacantRooms(std::vector<RoomInfo>& rooms, const std::string &datebegin, const std::string &dateend, uint32_t capacity, uint32_t startPrice, uint32_t endPrice,
                                       uint32_t startRating, uint32_t endRating, const std::string &room_type, uint32_t hotel_id,uint32_t role);
    static ResponseCode getVacantRoomsGuest(std::vector<RoomInfo>& rooms, const std::string &datebegin, const std::string &dateend, uint32_t capacity, uint32_t startPrice, uint32_t endPrice,
                                            uint32_t startRating, uint32_t endRating, const std::string &room_type, const std::string &hotel_type,uint32_t role);
    static ResponseCode getHotelOrders(std::vector<OrderInfo> &orders, uint32_t hotelid, uint32_t role);
    static ResponseCode editEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                     const std::string phone, int64_t salary, uint32_t position, int32_t hotelid, uint32_t role);
    static ResponseCode editHotelRoom(uint32_t id, uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role);
    static ResponseCode deleteEmployee(int64_t user_id, uint32_t role);
    static ResponseCode deleteRoom(int64_t hotel_id, uint32_t role);
    static ResponseCode getRoomTypes(std::vector<std::string>& types, uint32_t role);
    static ResponseCode getHotelTypes(std::vector<HotelType> &types, uint32_t role);
    static ResponseCode addHotelRoom(uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role);
    static ResponseCode makeOrder(const std::string &datebegin, const std::string &dateend, uint32_t idroom, uint32_t idemployee, uint32_t idguest, uint32_t role);

};

#endif // HOTELPOSTGRESMANAGER_H
