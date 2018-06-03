#ifndef MANAGERLOGIC_H
#define MANAGERLOGIC_H

#include "define.h"
#include "Protobuf/Message.pb.h"

class ManagerLogic
{
public:
    static ResponseCode getHotelEmployees(network::EmployeesMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode getHotelRooms(network::RoomsMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode editEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode editHotelRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode deleteEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode deleteRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode getRoomTypes(network::RoomTypesMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode getHotelTypes(network::HotelTypesMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode getHotelOrders(network::OrdersMessageResponse *responce, const network::RequestContext &request);
    static ResponseCode addHotelRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request);
    static uint32_t positionToInt(const std::string& pos);
};

#endif // MANAGERLOGIC_H
