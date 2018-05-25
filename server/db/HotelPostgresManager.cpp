#include "HotelPostgresManager.h"
#include "Types/EmployeeInfo.h".h"
#include "Types/GuestInfo.h"


ResponseCode HotelPostgresManager::createGuest(int64_t user_id,
                                               const std::string fname, const std::string sname, const std::string lname,
                                               const std::string phone, const std::string passport)
{
    ResponseCode result = ResponseCode::status_internal_error;
    
    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getGuestConnection();
            
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }
            
            if(!DBHelper::getDBHelper().isPrepared("CreateUser"))
            {
                connection->prepare("CreateUser",
                                    "INSERT INTO guest(id,firstname, secondname, lastname, phonenumber, passportnumber) "
                                    "VALUES ($1, $2::varchar, $3::varchar, $4::varchar, $5::varchar, $6::varchar);");
            }
            
            pqxx::work work(*connection, "CreateUser");
            
            work.prepared("CreateUser")(user_id)(fname)(sname)(lname)(phone)(passport).exec();
            
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

ResponseCode HotelPostgresManager::getEmployeeInfo(int64_t user_id, EmployeeInfo& eInfo)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getManagerConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getEmployee"))
            {
                connection->prepare("getEmployee",
                                    "SELECT employee.id,firstname,secondname,lastname,phonenumber,salary,name,idhotel"
                                    " from employeeposition INNER JOIN employee ON (idposition=employeeposition.id) WHERE $1 = employee.id;");
            }

            pqxx::work work(*connection, "getEmployee");

            pqxx::result res = work.prepared("getEmployee")(user_id).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            eInfo.parse_from_hn_employee(res[0]);
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

ResponseCode HotelPostgresManager::getGuestInfo(int64_t user_id, GuestInfo& gInfo)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getGuestConnection(); // sadasdasdas

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getGuest"))
            {
                connection->prepare("getEmployee",
                                    "SELECT * from guest where $1 = id;");
            }

            pqxx::work work(*connection, "getGuest");

            pqxx::result res = work.prepared("getGuest")(user_id).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            gInfo.parse_from_hn_guest(res[0]);
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getGuestInfo err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}
