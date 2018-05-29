#ifndef GLOBALPARAMS_H
#define GLOBALPARAMS_H

#include <string>

struct postgres_role
{
    std::string username;
    std::string password;
    std::string dbname;
};

class GlobalsParams
{
    static std::string listenChannelAddress;
    static int listenChannelPort;

    static postgres_role guestRole;
    static postgres_role adminRole;
    static postgres_role managerRole;
    static postgres_role receptionistRole;
    static postgres_role authRole;

    static std::string postgres_host;
    static std::string postgres_port;
    static std::string postgres_default_solt;

public:
    static std::string getListenChannelAddress();
    static void setListenChannelAddress(const std::string &value);
    static int getListenChannelPort();
    static void setListenChannelPort(int value);



    static std::string getPostgres_port();
    static void setPostgres_port(const std::string &value);
    static std::string getPostgres_host();
    static void setPostgres_host(const std::string &value);
    static std::string getPostgres_default_solt();
    static void setPostgres_default_solt(const std::string &value);

    static postgres_role getGuestRole();
    static void setGuestRole(const postgres_role &value);

    static postgres_role getAdminRole();
    static void setAdminRole(const postgres_role &value);

    static postgres_role getManagerRole();
    static void setManagerRole(const postgres_role &value);

    static postgres_role getReceptionistRole();
    static void setReceptionistRole(const postgres_role &value);

    static postgres_role getAuthRole();
    static void setAuthRole(const postgres_role &value);
};
#endif // GLOBALPARAMS_H
