#ifndef SESSIONMANAGERPOSTGRES_H
#define SESSIONMANAGERPOSTGRES_H

#include "DBHelper.h"
#include "define.h"

class SessionManagerPostgres
{
public:
    static ResponseCode createSession(int64_t user_id, std::string& session_id);
    static ResponseCode updateSessionToNow(std::string& session_id);
    static ResponseCode isExistSession(std::string& session_id);
};

#endif // SESSIONMANAGERPOSTGRES_H
