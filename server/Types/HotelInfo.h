#ifndef HOTELINFO_H
#define HOTELINFO_H

#include "define.h"
#include "pqxx/tuple"

struct HotelInfo
{
    uint64_t hotel_id;
    std::string name;
    std::string city;
    std::string street;
    std::string phone;
    std::string email;
    uint64_t stars;
    std::string type;

    void parse_from_hn_hotel(const pqxx::tuple& value);
};

#endif // HOTELINFO_H
