#include "AuthPostgresManager.h"
#include "Types/AuthUserInfo.h"

ResponseCode AuthPostgresManager::isUserExist(const std::string &login, bool &exist)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAuthConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("IsUserExist"))
            {
                connection->prepare("IsUserExist",
                                   "SELECT COUNT(*) FROM server_users WHERE user_login = $1::varchar");
            }

            pqxx::work work(*connection, "IsUserExist");

            pqxx::result query_result = work.prepared("IsUserExist")(login).exec();

            exist = query_result[0][0].as<int>() > 0;

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query IsUserExist err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode AuthPostgresManager::createUser(const std::string &login, const std::string pass, const std::string& salt, int64_t& user_id, uint32_t role)
{

    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAuthConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("CreateUser"))
            {
                connection->prepare("CreateUser",
                                   "INSERT INTO server_users(user_id, user_login, password, creation_time, salt, role) "
                                   "VALUES (DEFAULT, $1::varchar, $2::varchar, DEFAULT, $3::varchar, $4) RETURNING user_id;");
            }

            pqxx::work work(*connection, "CreateUser");

            pqxx::result res = work.prepared("CreateUser")(login)(pass)(salt)(role).exec();

            user_id = res[0][0].as<int64_t>();

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query RememberMailChain err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode AuthPostgresManager::getUser(const std::string &login, AuthUserInfo &uInfo)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAuthConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getUser"))
            {
                connection->prepare("getUser",
                                    " SELECT user_login, password, user_id, salt, role "
                                    "FROM server_users WHERE user_login = $1::varchar;");
            }

            pqxx::work work(*connection, "getUser");

            pqxx::result res = work.prepared("getUser")(login).exec();

            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }

            uInfo.parse_from_auth_db(res[0]);

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getUser err: " << e.what());
            break;
        }
    }
    while(false);

}
