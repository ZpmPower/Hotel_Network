#include "SessionManagerPostgres.h"

ResponseCode SessionManagerPostgres::createSession(const int64_t user_id, std::string &session_id)
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

                if(!DBHelper::getDBHelper().isPrepared("createSession"))
                {
                    connection->prepare("createSession",
                                       "INSERT INTO server_session(session, user_id, end_date) "
                                       "VALUES ($1::varchar, $2, DEFAULT);");
                }

                pqxx::work work(*connection, "createSession");

                work.prepared("createSession")(session_id)(user_id).exec();

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query createSession err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}

ResponseCode SessionManagerPostgres::getSession(const uint64_t &user_id, std::string &session_id)
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

                if(!DBHelper::getDBHelper().isPrepared("getSession"))
                {
                    connection->prepare("getSession",
                                       "SELECT session FROM server_session "
                                       "WHERE user_id = $1;");
                }

                pqxx::work work(*connection, "getSession");

                pqxx::result res = work.prepared("getSession")(user_id).exec();

                if(res.empty())
                {
                    result = ResponseCode::status_does_not_exist;
                    break;
                }

                session_id = res[0][0].as<std::string>();

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query getSession err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}

ResponseCode SessionManagerPostgres::isExistSession(const uint64_t &user_id, bool &isExist)
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

                if(!DBHelper::getDBHelper().isPrepared("isSessionExist"))
                {
                    connection->prepare("isSessionExist",
                                       "SELECT COUNT(*) FROM server_session "
                                       "WHERE user_id = $1;");
                }

                pqxx::work work(*connection, "isSessionExist");

                pqxx::result res = work.prepared("isSessionExist")(user_id).exec();

                if(res.empty())
                {
                    isExist = false;
                }
                else
                {
                    isExist = res[0][0].as<int>() > 0;
                }

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query isSessionExist err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}


ResponseCode SessionManagerPostgres::getUserBySession(const std::string &session_id, uint64_t &user_id)
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

                if(!DBHelper::getDBHelper().isPrepared("getUserBySession"))
                {
                    connection->prepare("getUserBySession",
                                       "SELECT user_id FROM server_session "
                                       "WHERE session = $1::varchar;");
                }

                pqxx::work work(*connection, "getUserBySession");

                pqxx::result res = work.prepared("getUserBySession")(session_id).exec();

                if(res.empty())
                {
                    result = ResponseCode::status_does_not_exist;
                    break;
                }

                user_id = res[0][0].as<uint64_t>();

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query getUserBySession err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}

ResponseCode SessionManagerPostgres::getRoleBySession(const std::string &session_id, uint32_t &role)
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

                if(!DBHelper::getDBHelper().isPrepared("getRoleBySession"))
                {
                    connection->prepare("getRoleBySession",
                                       "SELECT role FROM server_session JOIN server_users "
                                       "ON server_session.user_id = server_users.user_id WHERE session =  $1::varchar;");
                }

                pqxx::work work(*connection, "getRoleBySession");

                pqxx::result res = work.prepared("getRoleBySession")(session_id).exec();

                if(res.empty())
                {
                    result = ResponseCode::status_does_not_exist;
                    break;
                }

                role = res[0][0].as<uint64_t>();

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query getRoleBySession err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}

ResponseCode SessionManagerPostgres::removeSession(const uint64_t &user_id)
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

                if(!DBHelper::getDBHelper().isPrepared("removeSessionByUser"))
                {
                    connection->prepare("removeSessionByUser",
                                       "DELETE FROM server_session "
                                       "WHERE user_id = $1;");
                }

                pqxx::work work(*connection, "removeSessionByUser");

                work.prepared("removeSessionByUser")(user_id).exec();

                work.commit();
                result = ResponseCode::status_success;
            }
            catch(const std::exception& e)
            {
                LOG_ERR("Failure: trying to query removeSessionByUser err: " << e.what());
                break;
            }
        }
        while(false);

        return result;
}
