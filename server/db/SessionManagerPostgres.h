#ifndef SESSIONMANAGERPOSTGRES_H
#define SESSIONMANAGERPOSTGRES_H

#include "DBHelper.h"
#include "define.h"

class SessionManagerPostgres
{
public:
    static ResponseCode createSession(const int64_t user_id, std::string& session_id);
    static ResponseCode isExistSession(const uint64_t &user_id, bool &isExist);
    static ResponseCode getSession(const uint64_t &user_id, std::string &session_id);
    static ResponseCode getUserBySession(const std::string &session_id, uint64_t &user_id);
    static ResponseCode getRoleBySession(const std::string &session_id, uint32_t &role);
    static ResponseCode removeSession(const uint64_t& user_id);
};

#endif // SESSIONMANAGERPOSTGRES_H
