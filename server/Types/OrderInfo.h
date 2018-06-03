#ifndef ORDERINFO_H
#define ORDERINFO_H

#include "define.h"
#include "pqxx/tuple"

struct OrderInfo
{
    uint64_t order_id;
    std::string startdate;
    std::string enddate;
    uint32_t idroom;
    uint32_t idemployee;
    uint32_t idguest;
    uint32_t idhotel;
    std::string employee_secondname;
    std::string guest_secondname;

    void parse_from_hn_order(const pqxx::tuple& value);
};

#endif // ORDERINFO_H
