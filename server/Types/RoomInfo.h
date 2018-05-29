#ifndef ROOMINFO_H
#define ROOMINFO_H


#include "define.h"
#include "pqxx/tuple"

struct RoomInfo
{
    uint64_t room_id;
    uint64_t places;
    uint64_t price;
    uint64_t rating;
    bool status;
    uint64_t floor;
    std::string type;
    uint64_t hotelID;

    void parse_from_hn_room(const pqxx::tuple& value);
};

#endif // ROOMINFO_H
