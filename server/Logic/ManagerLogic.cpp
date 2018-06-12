#include "ManagerLogic.h"
#include "db/HotelPostgresManager.h"
#include "Types/GuestInfo.h"
#include "Types/EmployeeInfo.h"
#include "Types/HotelInfo.h"
#include "Types/RoomInfo.h"
#include "Types/OrderInfo.h"
#include "Types/HotelType.h"
#include "Types/GuestOrder.h"
#include "db/SessionManagerPostgres.h"
#include "db/AuthPostgresManager.h"

ResponseCode ManagerLogic::getHotelEmployees(network::EmployeesMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<EmployeeInfo> employees;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getHotelEmployees(employees,request.hotel_id().hotelid(),role);
        for(EmployeeInfo info: employees)
        {

            network::EmployeeInfo* employee = responce->add_employees();
            employee->set_firstname(info.firstN);
            employee->set_secondname(info.secondN);
            employee->set_lastname(info.lastN);
            employee->set_phonenumber(info.phone);
            employee->set_salary(info.salary);
            employee->set_position(info.position);
            employee->set_id(info.employee_id);
            employee->set_hotelid(info.hotel_id);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getHotelRooms(network::RoomsMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<RoomInfo> rooms;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getHotelRooms(rooms,request.hotel_id().hotelid(),role);
        for(RoomInfo info: rooms)
        {

            network::RoomInfo* room = responce->add_rooms();
            room->set_places(info.places);
            room->set_price(info.price);
            room->set_rating(info.rating);
            room->set_status(info.status);
            room->set_floor(info.floor);
            room->set_type(info.type);
            room->set_hotelid(info.hotelID);
            room->set_id(info.room_id);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getVacantRooms(network::RoomsMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<RoomInfo> rooms;
        uint32_t role;
        network::VacantRooms vacant = request.data();
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getVacantRooms(rooms,vacant.datebegin(),vacant.dateend(),vacant.places(),vacant.begin_price(),vacant.end_price(),
                                                      vacant.begin_rating(),vacant.end_rating(),vacant.room_type(),vacant.hotelid(),role);
        for(RoomInfo info: rooms)
        {

            network::RoomInfo* room = responce->add_rooms();
            room->set_places(info.places);
            room->set_price(info.price);
            room->set_rating(info.rating);
            room->set_status(info.status);
            room->set_floor(info.floor);
            room->set_type(info.type);
            room->set_hotelid(info.hotelID);
            room->set_id(info.room_id);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getVacantRoomsGuest(network::RoomsMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<RoomInfo> rooms;
        uint32_t role;
        network::VacantRoomsGuest vacant = request.vacant_rooms_guest();
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getVacantRoomsGuest(rooms,vacant.datebegin(),vacant.dateend(),vacant.places(),vacant.begin_price(),vacant.end_price(),
                                                      vacant.begin_rating(),vacant.end_rating(),vacant.room_type(),vacant.hotel_type(),role);
        for(RoomInfo info: rooms)
        {

            network::RoomInfo* room = responce->add_rooms();
            room->set_places(info.places);
            room->set_price(info.price);
            room->set_rating(info.rating);
            room->set_status(info.status);
            room->set_floor(info.floor);
            room->set_type(info.type);
            room->set_hotelid(info.hotelID);
            room->set_id(info.room_id);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::editEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::EmployeeInfo info = request.employee_info();
        uint32_t position = positionToInt(info.position());

        result = HotelPostgresManager::editEmployee(info.id(),info.firstname(),info.secondname(),info.lastname(),info.phonenumber(),info.salary(),position,info.hotelid(),role);
        responce->set_messagetext("Success");
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::editGuest(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::GuestInfo info = request.guest_info();

        result = HotelPostgresManager::editGuest(info.id(),info.firstname(),info.secondname(),info.lastname(),info.phonenumber(),info.passport(),role);
        responce->set_messagetext("Success");
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::editHotelRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::RoomInfo info = request.room_info();

        result = HotelPostgresManager::editHotelRoom(info.id(),info.places(),info.price(),info.rating(),info.status(),info.floor(),info.type(),info.hotelid(),role);
        responce->set_messagetext("Success");
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::deleteEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::EmployeeInfo info = request.employee_info();

        if(AuthPostgresManager::deleteUser(info.id()) != ResponseCode::status_success)
        {
            responce->set_messagetext("Cannot delete user!");
            responce->set_status(false);
        }
        else
        {
            HotelPostgresManager::deleteEmployee(info.id(),role);
            responce->set_messagetext("Success");
            responce->set_status(true);

        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::deleteRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::RoomInfo info = request.room_info();

        HotelPostgresManager::deleteRoom(info.id(),role);
        responce->set_messagetext("Success");
        responce->set_status(true);
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getRoomTypes(network::RoomTypesMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<std::string> types;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getRoomTypes(types,role);

        for(std::string info: types)
        {
            std::string* s = responce->add_types();
            *s = info;
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getHotelTypes(network::HotelTypesMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<HotelType> types;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getHotelTypes(types,role);
        for(HotelType info: types)
        {

            network::HotelType* type = responce->add_types();
            type->set_id(info.id);
            type->set_name(info.name);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getGuestById(network::GuestInfo *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        GuestInfo guestInfo;
        result = HotelPostgresManager::getGuestInfo(request.hotel_id().hotelid(),guestInfo);
        responce->set_firstname(guestInfo.firstN);
        responce->set_secondname(guestInfo.secondN);
        responce->set_lastname(guestInfo.lastN);
        responce->set_phonenumber(guestInfo.phone);
        responce->set_passport(guestInfo.passport);
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getHotelOrders(network::OrdersMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<OrderInfo> orders;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getHotelOrders(orders,request.hotel_id().hotelid(),role);
        for(OrderInfo info: orders)
        {

            network::OrderInfo* order = responce->add_orders();
            order->set_id(info.order_id);
            order->set_startdate(info.startdate);
            order->set_enddate(info.enddate);
            order->set_idroom(info.idroom);
            order->set_employee_id(info.idemployee);
            order->set_guest_id(info.idguest);
            order->set_hotelid(info.idhotel);
            order->set_employee_secondname(info.employee_secondname);
            order->set_guest_secondname(info.guest_secondname);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::addHotelRoom(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::RoomInfo room = request.room_info();
        result = HotelPostgresManager::addHotelRoom(room.places(),room.price(),room.rating(),room.status(),room.floor(),room.type(),room.hotelid(),role);
        responce->set_messagetext("Success");
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::makeOrder(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::MakeOrderInfo info = request.make_order_info();
        result = HotelPostgresManager::makeOrder(info.startdate(),info.enddate(),info.idroom(),info.idemployee(),info.idguest(),role);
        responce->set_messagetext("Success");
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::countHotelRooms(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::HotelId info = request.hotel_id();
        uint32_t countRooms=0;
        result = HotelPostgresManager::countHotelRooms(countRooms,info.hotelid(),role);
        responce->set_messagetext(std::to_string(countRooms));
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::countHotelEmployeess(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::HotelId info = request.hotel_id();
        uint32_t countEmployees=0;
        result = HotelPostgresManager::countHotelEmployees(countEmployees,info.hotelid(),role);
        responce->set_messagetext(std::to_string(countEmployees));
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::countHotelOrders(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::HotelId info = request.hotel_id();
        uint32_t countOrders=0;
        result = HotelPostgresManager::countHotelOrders(countOrders,info.hotelid(),role);
        responce->set_messagetext(std::to_string(countOrders));
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::getCurrentGuestsRR(network::GuestOrdersMessageResponse *response, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        std::vector<GuestOrder> guestsOrders;
        result = HotelPostgresManager::getCurrentGuests(guestsOrders,request.hotel_id().hotelid());
        for(GuestOrder info: guestsOrders)
        {

            network::GuestOrderInfo* guest = response->add_orders();
            guest->set_guest_firstname(info.firstN);
            guest->set_guest_secondname(info.secondN);
            guest->set_guest_lastname(info.lastN);
            guest->set_startdate(info.startdate);
            guest->set_enddate(info.enddate);
            guest->set_idroom(info.idroom);
        }
    }
     while(false);

     return result;
}

ResponseCode ManagerLogic::avgResidenceTime(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::HotelId info = request.hotel_id();
        double avgTime=0;
        result = HotelPostgresManager::avgResidenceTime(avgTime,info.hotelid(),role);
        std::string s;
            std::stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << avgTime;

            s = sstream.str();

        responce->set_messagetext(s);
        responce->set_status(true);

    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::avgRoomRating(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::HotelId info = request.hotel_id();
        double avgRating=0;
        result = HotelPostgresManager::avgRoomRating(avgRating,info.hotelid(),role);
        std::string s;
            std::stringstream sstream;
            sstream.setf(std::ios::fixed);
            sstream.precision(2);
            sstream << avgRating;

            s = sstream.str();

        responce->set_messagetext(s);
        responce->set_status(true);

    }
     while(false);
     return result;
}

uint32_t ManagerLogic::positionToInt(const std::string &pos)
{
    if(pos=="Manager") return 1;
    if(pos=="Receptionist") return 2;
    if(pos=="Admin") return 0;
    if(pos=="Guest") return 3;
}
