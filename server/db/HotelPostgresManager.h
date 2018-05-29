#ifndef HOTELPOSTGRESMANAGER_H
#define HOTELPOSTGRESMANAGER_H

#include "DBHelper.h"
#include "define.h"

struct GuestInfo;
struct EmployeeInfo;
struct HotelInfo;
struct RoomInfo;

class HotelPostgresManager
{
public:
    static ResponseCode createGuest(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                    const std::string phone, const std::string passport);
    static ResponseCode createEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                    const std::string phone, int64_t salary, int32_t position, int32_t hotelid);
    static ResponseCode getEmployeeInfo(int64_t user_id,EmployeeInfo &eInfo);
    static ResponseCode getGuestInfo(int64_t user_id, GuestInfo &gInfo);
    static ResponseCode getGuests(std::vector<GuestInfo>& guests);
    static ResponseCode getEmployees(std::vector<EmployeeInfo>& employees);
    static ResponseCode getHotels(std::vector<HotelInfo>& hotels);
    static ResponseCode getRooms(std::vector<RoomInfo>& rooms);

};

#endif // HOTELPOSTGRESMANAGER_H
