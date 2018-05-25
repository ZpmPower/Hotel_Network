#include "GlobalParams.h"

std::string GlobalsParams::listenChannelAddress;
int GlobalsParams::listenChannelPort;

postgres_role GlobalsParams::guestRole;
postgres_role GlobalsParams::adminRole;
postgres_role GlobalsParams::managerRole;
postgres_role GlobalsParams::receptionistRole;
postgres_role GlobalsParams::authRole;

std::string GlobalsParams::postgres_host;
std::string GlobalsParams::postgres_port;
std::string GlobalsParams::postgres_default_solt;

int GlobalsParams::getListenChannelPort()
{
    return listenChannelPort;
}

void GlobalsParams::setListenChannelPort(int value)
{
    listenChannelPort = value;
}

void GlobalsParams::setAuthRole(const postgres_role &value)
{
    authRole = value;
}

std::string GlobalsParams::getPostgres_port()
{
    return postgres_port;
}

void GlobalsParams::setPostgres_port(const std::string &value)
{
    postgres_port = value;
}

std::string GlobalsParams::getPostgres_host()
{
    return postgres_host;
}

void GlobalsParams::setPostgres_host(const std::string &value)
{
    postgres_host = value;
}

std::string GlobalsParams::getPostgres_default_solt()
{
    return postgres_default_solt;
}

void GlobalsParams::setPostgres_default_solt(const std::string &value)
{
    postgres_default_solt = value;
}

postgres_role GlobalsParams::getGuestRole()
{
    return guestRole;
}

void GlobalsParams::setGuestRole(const postgres_role &value)
{
    guestRole = value;
}

postgres_role GlobalsParams::getAdminRole()
{
    return adminRole;
}

void GlobalsParams::setAdminRole(const postgres_role &value)
{
    adminRole = value;
}

postgres_role GlobalsParams::getManagerRole()
{
    return managerRole;
}

void GlobalsParams::setManagerRole(const postgres_role &value)
{
    managerRole = value;
}

postgres_role GlobalsParams::getReceptionistRole()
{
    return receptionistRole;
}

void GlobalsParams::setReceptionistRole(const postgres_role &value)
{
    receptionistRole = value;
}

postgres_role GlobalsParams::getAuthRole()
{
    return authRole;
}

std::string GlobalsParams::getListenChannelAddress()
{
    return listenChannelAddress;
}

void GlobalsParams::setListenChannelAddress(const std::string &value)
{
    listenChannelAddress = value;
}
