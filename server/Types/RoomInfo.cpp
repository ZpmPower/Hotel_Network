#include "RoomInfo.h"

void RoomInfo::parse_from_hn_room(const pqxx::tuple &value)
{
    room_id = value["id"].as<uint64_t>();
    places = value["places"].as<uint64_t>();
    price = value["price"].as<uint64_t>();
    rating = value["rating"].as<uint64_t>();
    status  = value["status"].as<bool>();
    floor = value["floor"].as<uint64_t>();
    type = value["type"].as<std::string>();
    hotelID = value["idhotel"].as<uint64_t>();
}
