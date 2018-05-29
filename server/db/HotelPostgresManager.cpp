#include "HotelPostgresManager.h"
#include "Types/EmployeeInfo.h"
#include "Types/GuestInfo.h"
#include "Types/HotelInfo.h"
#include "Types/RoomInfo.h"


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

ResponseCode HotelPostgresManager::createEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname, const std::string phone,
                                                  int64_t salary, int32_t position, int32_t hotelid)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAdminConnection();
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("CreateEmployee"))
            {
                connection->prepare("CreateEmployee",
                                    "INSERT INTO employee(id,firstname, secondname, lastname, phonenumber, salary,idposition,idhotel) "
                                    "VALUES ($1, $2::varchar, $3::varchar, $4::varchar, $5::varchar, $6,$7,$8);");
            }

            pqxx::work work(*connection, "CreateEmployee");

            work.prepared("CreateEmployee")(user_id)(fname)(sname)(lname)(phone)(salary)(position)(hotelid).exec();

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
            db_connection_ptr connection = DBHelper::getAdminConnection();

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
                connection->prepare("getGuest",
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

ResponseCode HotelPostgresManager::getGuests(std::vector<GuestInfo> &guests)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAdminConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getGuests"))
            {
                connection->prepare("getGuests",
                                    "SELECT * from guest;");
            }

            pqxx::work work(*connection, "getGuests");

            pqxx::result res = work.prepared("getGuests").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                GuestInfo gInfo;
                gInfo.parse_from_hn_guest(value);
                guests.push_back(gInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getGuests err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getEmployees(std::vector<EmployeeInfo> &employees)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAdminConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getEmployees"))
            {
                connection->prepare("getEmployees",
                                    "SELECT employee.id,firstname,secondname,lastname,phonenumber,salary, name, idhotel from employee "
                                    "INNER JOIN employeeposition ON idposition=employeeposition.id;");
            }

            pqxx::work work(*connection, "getEmployees");

            pqxx::result res = work.prepared("getEmployees").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                EmployeeInfo eInfo;
                eInfo.parse_from_hn_employee(value);
                employees.emplace_back(eInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getGuests err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getHotels(std::vector<HotelInfo> &hotels)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAdminConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getHotels"))
            {
                connection->prepare("getHotels",
                                    "SELECT hotel.id,hotel.name,city,street,phonenumber,email, stars, hoteltype.name AS type from hotel "
                                    "JOIN hoteltype ON idtype=hoteltype.id;");
            }

            pqxx::work work(*connection, "getHotels");

            pqxx::result res = work.prepared("getHotels").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                HotelInfo hInfo;
                hInfo.parse_from_hn_hotel(value);
                hotels.emplace_back(hInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getGuests err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getRooms(std::vector<RoomInfo> &rooms)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = DBHelper::getAdminConnection();

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getRooms"))
            {
                connection->prepare("getRooms",
                                    "SELECT room.id,places,price,rating,status,floor, roomtype.name AS type, idhotel from room "
                                    "JOIN roomtype ON idtype=roomtype.id;");
            }

            pqxx::work work(*connection, "getRooms");

            pqxx::result res = work.prepared("getRooms").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                RoomInfo rInfo;
                rInfo.parse_from_hn_room(value);
                rooms.emplace_back(rInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getGuests err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}
