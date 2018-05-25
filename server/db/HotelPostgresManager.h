#ifndef HOTELPOSTGRESMANAGER_H
#define HOTELPOSTGRESMANAGER_H

#include "DBHelper.h"
#include "define.h"

struct GuestInfo;
struct EmployeeInfo;

class HotelPostgresManager
{
public:
    static ResponseCode createGuest(int64_t user_id,
                                    const std::string fname, const std::string sname, const std::string lname,
                                    const std::string phone, const std::string passport);
    static ResponseCode getEmployeeInfo(int64_t user_id,EmployeeInfo &eInfo);
    static ResponseCode getGuestInfo(int64_t user_id, GuestInfo &gInfo);
};

#endif // HOTELPOSTGRESMANAGER_H
