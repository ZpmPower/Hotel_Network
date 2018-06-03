#include "OrderInfo.h"

void OrderInfo::parse_from_hn_order(const pqxx::tuple &value)
{
    order_id = value["id"].as<uint32_t>();
    startdate = value["startdate"].as<std::string>();
    enddate = value["enddate"].as<std::string>();
    idroom = value["idroom"].as<uint32_t>();
    idemployee  = value["idemployee"].as<uint32_t>();
    idguest = value["idguest"].as<uint32_t>();
    employee_secondname = value["secondname"].as<std::string>();
    guest_secondname = value["guest"].as<std::string>();
    idhotel = value["idhotel"].as<uint32_t>();
}
