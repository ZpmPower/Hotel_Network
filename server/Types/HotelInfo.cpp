#include "HotelInfo.h"

void HotelInfo::parse_from_hn_hotel(const pqxx::tuple &value)
{
    hotel_id = value["id"].as<uint64_t>();
    name = value["name"].as<std::string>();
    city = value["city"].as<std::string>();
    street = value["street"].as<std::string>();
    phone  = value["phonenumber"].as<std::string>();
    email = value["email"].as<std::string>();
    stars = value["stars"].as<uint64_t>();
    type = value["type"].as<std::string>();
}
