#ifndef AUTHLOGIC_H
#define AUTHLOGIC_H

#include "define.h"
#include "Protobuf/Message.pb.h"

class AuthLogic
{
public:
    static ResponseCode createUser(const network::RegisterMessage& authData, network::RegisterMessageResponse *response);
    static ResponseCode createEmployee(const network::RegisterEmployeeMessage& authData, network::RegisterMessageResponse *response);
    static ResponseCode authUser(const network::AuthMessage& authData, network::AuthMessageResponse *response);
};

#endif // AUTHLOGIC_H
