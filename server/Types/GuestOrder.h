#ifndef GUESTORDER_H
#define GUESTORDER_H


#include "define.h"
#include "pqxx/tuple"

struct GuestOrder
{
    uint64_t guest_id;
    std::string firstN;
    std::string secondN;
    std::string lastN;
    std::string startdate;
    std::string enddate;
    uint32_t idroom;
    void parse_from_hn_guest_order(const pqxx::tuple& value);
};

#endif // GUESTORDER_H
