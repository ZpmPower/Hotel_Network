#include "AuthUserInfo.h"

void AuthUserInfo::parse_from_auth_db(const pqxx::tuple &value)
{
    user_id = value["user_id"].as<uint64_t>();
    pass = value["password"].as<std::string>();
    user_login = value["user_login"].as<std::string>();
    salt = value["salt"].as<std::string>();
}

