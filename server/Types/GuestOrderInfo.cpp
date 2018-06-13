#include "GuestOrderInfo.h"


void GuestOrderInfo::parse_from_hn_order(const pqxx::tuple &value)
{
    startdate = value["startdate"].as<std::string>();
    enddate = value["enddate"].as<std::string>();
    idroom = value["idroom"].as<uint32_t>();
    idhotel = value["idhotel"].as<uint32_t>();
}
