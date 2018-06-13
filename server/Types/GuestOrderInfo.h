#ifndef GUESTORDERINFO_H
#define GUESTORDERINFO_H


#include "define.h"
#include "pqxx/tuple"

struct GuestOrderInfo
{
    std::string startdate;
    std::string enddate;
    uint32_t idroom;
    uint32_t idhotel;

    void parse_from_hn_order(const pqxx::tuple& value);
};


#endif // GUESTORDERINFO_H
