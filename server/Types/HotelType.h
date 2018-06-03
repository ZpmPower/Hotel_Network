#ifndef HOTELTYPE_H
#define HOTELTYPE_H


#include "define.h"
#include "pqxx/tuple"

struct HotelType
{
    uint32_t id;
    std::string name;

    void parse_from_hn_hoteltypes(const pqxx::tuple& value);
};

#endif // HOTELTYPE_H
