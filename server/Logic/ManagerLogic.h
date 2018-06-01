#ifndef MANAGERLOGIC_H
#define MANAGERLOGIC_H

#include "define.h"
#include "Protobuf/Message.pb.h"

class ManagerLogic
{
public:
    static ResponseCode getHotelEmployees(network::EmployeesMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode editEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static uint32_t positionToInt(const std::string& pos);
};

#endif // MANAGERLOGIC_H
