#ifndef ADMINLOGIC_H
#define ADMINLOGIC_H

#include "define.h"
#include "Protobuf/Message.pb.h"

class AdminLogic
{
public:
    static ResponseCode getGuests(network::GuestsMessageResponse *response);
    static ResponseCode getEmployees(network::EmployeesMessageResponse *responce);
    static ResponseCode getHotels(network::HotelsMessageResponse *responce);
    static ResponseCode getRooms(network::RoomsMessageResponse *responce);
};

#endif // ADMINLOGIC_H
