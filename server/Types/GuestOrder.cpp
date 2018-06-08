#include "GuestOrder.h"


void GuestOrder::parse_from_hn_guest_order(const pqxx::tuple &value)
{
    guest_id = value["id"].as<uint64_t>();
    firstN = value["firstname"].as<std::string>();
    secondN = value["secondname"].as<std::string>();
    lastN = value["lastname"].as<std::string>();
    idroom = value["id"].as<std::uint32_t>();
    startdate = value["startdate"].as<std::string>();
    enddate = value["enddate"].as<std::string>();
}
