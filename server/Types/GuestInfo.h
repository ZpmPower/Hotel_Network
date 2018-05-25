#ifndef GUESTINFO_H
#define GUESTINFO_H

#include "define.h"
#include "pqxx/tuple"

struct GuestInfo
{
    uint64_t guest_id;
    std::string firstN;
    std::string secondN;
    std::string lastN;
    std::string phone;
    std::string passport;

    void parse_from_hn_guest(const pqxx::tuple& value);
};

#endif // GUESTINFO_H
