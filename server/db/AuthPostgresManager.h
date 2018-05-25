#ifndef AUTHPOSTGRESMANAGER_H
#define AUTHPOSTGRESMANAGER_H

#include "DBHelper.h"
#include "define.h"

struct AuthUserInfo;

class AuthPostgresManager
{
public:
    static ResponseCode isUserExist(const std::string& login, bool& exist);
    static ResponseCode createUser(const std::string& login, const std::string pass, const std::string &salt, int64_t &user_id);
    static ResponseCode getUser(const std::string& login, AuthUserInfo& uInfo);
    //static ResponseCode get
};

#endif // AUTHPOSTGRESMANAGER_H
