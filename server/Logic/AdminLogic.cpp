#include "AdminLogic.h"
#include "db/HotelPostgresManager.h"
#include "Types/GuestInfo.h"
#include "Types/EmployeeInfo.h"
#include "Types/HotelInfo.h"
#include "Types/RoomInfo.h"

ResponseCode AdminLogic::getGuests(network::GuestsMessageResponse *response)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        std::vector<GuestInfo> guests;
        result = HotelPostgresManager::getGuests(guests);
        for(GuestInfo info: guests)
        {

            network::GuestInfo* guest = response->add_guests();
            guest->set_firstname(info.firstN);
            guest->set_secondname(info.secondN);
            guest->set_lastname(info.lastN);
            guest->set_phonenumber(info.phone);
            guest->set_passport(info.passport);
            guest->set_id(info.guest_id);
        }
    }
     while(false);

     return result;
}

ResponseCode AdminLogic::getEmployees(network::EmployeesMessageResponse *responce)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        std::vector<EmployeeInfo> employees;
        result = HotelPostgresManager::getEmployees(employees);
        for(EmployeeInfo info: employees)
        {

            network::EmployeeInfo* employee = responce->add_employees();
            employee->set_firstname(info.firstN);
            employee->set_secondname(info.secondN);
            employee->set_lastname(info.lastN);
            employee->set_phonenumber(info.phone);
            employee->set_salary(info.salary);
            employee->set_position(info.position);
            employee->set_hotelid(info.hotel_id);
        }
    }
     while(false);
     return result;
}

ResponseCode AdminLogic::getHotels(network::HotelsMessageResponse *responce)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        std::vector<HotelInfo> hotels;
        result = HotelPostgresManager::getHotels(hotels);
        for(HotelInfo info: hotels)
        {

            network::HotelInfo* hotel = responce->add_hotels();
            hotel->set_name(info.name);
            hotel->set_city(info.city);
            hotel->set_street(info.street);
            hotel->set_phonenumber(info.phone);
            hotel->set_email(info.email);
            hotel->set_stars(info.stars);
            hotel->set_type(info.type);
        }
    }
     while(false);
     return result;
}

ResponseCode AdminLogic::getRooms(network::RoomsMessageResponse *responce)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        std::vector<RoomInfo> rooms;
        result = HotelPostgresManager::getRooms(rooms);
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
        }
    }
     while(false);
     return result;
}
