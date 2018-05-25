#include "ConfigManager.h"
#include <string>
#include "GlobalParams.h"
#include "define.h"

void parse_db_params(const libconfig::Config& cfg);

ConfigManager::ConfigManager()
{
}

bool ConfigManager::readConfigFIle(const std::string &path)
{
    try
    {
        conf.readFile(path.c_str());
        return true;
    }
    catch(libconfig::ParseException e)
    {
        LOG_ERR("Failure read config file: " << e.what());
    }
    return false;
}

void ConfigManager::parseConfigs()
{
    if(GlobalsParams::getListenChannelAddress().empty())
        GlobalsParams::setListenChannelAddress(conf.lookup("application.network.listen_address"));

    if(GlobalsParams::getListenChannelPort() == 0)
        GlobalsParams::setListenChannelPort(conf.lookup("application.network.listen_port"));

    parse_db_params(conf);
}


void parse_db_params(const libconfig::Config& cfg)
{
    postgres_role admin;
    admin.dbname = cfg.lookup("application.postgres.admin.dbname").c_str();
    admin.password = cfg.lookup("application.postgres.admin.password").c_str();
    admin.username = cfg.lookup("application.postgres.admin.user").c_str();

    GlobalsParams::setAdminRole(admin);

    postgres_role guest;
    guest.dbname = cfg.lookup("application.postgres.guest.dbname").c_str();
    guest.password = cfg.lookup("application.postgres.guest.password").c_str();
    guest.username = cfg.lookup("application.postgres.guest.user").c_str();

    GlobalsParams::setGuestRole(guest);

    postgres_role auth;
    auth.dbname = cfg.lookup("application.postgres.auth.dbname").c_str();
    auth.password = cfg.lookup("application.postgres.auth.password").c_str();
    auth.username = cfg.lookup("application.postgres.auth.user").c_str();

    GlobalsParams::setAuthRole(auth);

    GlobalsParams::setPostgres_host(cfg.lookup("application.postgres.host"));
    GlobalsParams::setPostgres_port(cfg.lookup("application.postgres.port"));
    GlobalsParams::setPostgres_default_solt(cfg.lookup("application.postgres.salt"));

    postgres_role manager;
    manager.dbname = cfg.lookup("application.postgres.manager.dbname").c_str();
    manager.password = cfg.lookup("application.postgres.manager.password").c_str();
    manager.username = cfg.lookup("application.postgres.manager.user").c_str();

    GlobalsParams::setAdminRole(manager);

    postgres_role receptionist;
    receptionist.dbname = cfg.lookup("application.postgres.receptionist.dbname").c_str();
    receptionist.password = cfg.lookup("application.postgres.receptionist.password").c_str();
    receptionist.username = cfg.lookup("application.postgres.receptionist.user").c_str();

    GlobalsParams::setAdminRole(receptionist);
}
