#ifndef AUTHUSERINFO_H
#define AUTHUSERINFO_H

#include "define.h"
#include "pqxx/tuple"

struct AuthUserInfo
{
    std::string user_login;
    std::string pass;
    uint64_t user_id;
    std::string salt;

    void parse_from_auth_db(const pqxx::tuple& value);
};

#endif // AUTHUSERINFO_H
