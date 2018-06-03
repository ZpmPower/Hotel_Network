#include "HotelType.h"



void HotelType::parse_from_hn_hoteltypes(const pqxx::tuple &value)
{
    id = value["id"].as<uint32_t>();
    name = value["name"].as<std::string>();
}
