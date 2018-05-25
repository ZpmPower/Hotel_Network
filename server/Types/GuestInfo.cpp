#include "GuestInfo.h"

void GuestInfo::parse_from_hn_guest(const pqxx::tuple &value)
{
    guest_id = value["id"].as<uint64_t>();
    firstN = value["firstname"].as<std::string>();
    secondN = value["secondname"].as<std::string>();
    lastN = value["lastname"].as<std::string>();
    phone  = value["phonenumber"].as<std::string>();
    passport = value["passportnumber"].as<std::string>();
}
