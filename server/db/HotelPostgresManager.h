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
    static ResponseCode getEmployees(std::vector<EmployeeInfo>& employees);
    static ResponseCode getHotelEmployees(std::vector<EmployeeInfo>& employees, uint32_t hotelid, uint32_t role);
    static ResponseCode getHotels(std::vector<HotelInfo>& hotels);
    static ResponseCode getRooms(std::vector<RoomInfo>& rooms);
    static ResponseCode getHotelRooms(std::vector<RoomInfo>& rooms, uint32_t hotelid, uint32_t role);
    static ResponseCode getHotelOrders(std::vector<OrderInfo> &orders, uint32_t hotelid, uint32_t role);
    static ResponseCode editEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                     const std::string phone, int64_t salary, uint32_t position, int32_t hotelid, uint32_t role);
    static ResponseCode editHotelRoom(uint32_t id, uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role);
    static ResponseCode deleteEmployee(int64_t user_id, uint32_t role);
    static ResponseCode deleteRoom(int64_t hotel_id, uint32_t role);
    static ResponseCode getRoomTypes(std::vector<std::string>& types, uint32_t role);
    static ResponseCode getHotelTypes(std::vector<HotelType> &types, uint32_t role);
    static ResponseCode addHotelRoom(uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role);

};

#endif // HOTELPOSTGRESMANAGER_H
