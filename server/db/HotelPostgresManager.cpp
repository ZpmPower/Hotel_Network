#include "HotelPostgresManager.h"
#include "Types/EmployeeInfo.h"
#include "Types/GuestInfo.h"
#include "Types/HotelInfo.h"
#include "Types/RoomInfo.h"
#include "Types/OrderInfo.h"
#include "Types/HotelType.h"
#include "Types/GuestOrder.h"


db_connection_ptr HotelPostgresManager::checkConnection(uint32_t role)
{
    switch (static_cast<Roles>(role))
    {
        case Roles::role_admin:
        {
            return DBHelper::getAdminConnection();
            break;
        }
        case Roles::role_guest:
        {
            return DBHelper::getGuestConnection();
            break;
        }
        case Roles::role_manager:
        {
            return DBHelper::getManagerConnection();
            break;
        }
        case Roles::role_receptionist:
        {
         return DBHelper::getReceptionistConnection();
         break;
        }

    }
}

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

ResponseCode HotelPostgresManager::getCurrentGuests(std::vector<GuestOrder> &guests, uint32_t hotel_id)
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

            if(!DBHelper::getDBHelper().isPrepared("getCurrentGuests"))
            {
                connection->prepare("getCurrentGuests",
                                    "SELECT guest.firstname,guest.secondname,guest.lastname,startdate,enddate,room.id FROM Room INNER JOIN Roomorder ON room.id=roomorder.idroom "
                                    "JOIN GUEST ON idguest=guest.id WHERE NOW() BETWEEN "
                                    "STARTDATE AND ENDDATE AND idhotel=$1;");
            }

            pqxx::work work(*connection, "getCurrentGuests");

            pqxx::result res = work.prepared("getCurrentGuests")(hotel_id).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                GuestOrder gInfo;
                gInfo.parse_from_hn_guest_order(value);
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

ResponseCode HotelPostgresManager::getHotelEmployees(std::vector<EmployeeInfo> &employees, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    LOG_INFO(hotelid);
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getHotelEmployees"))
            {
                connection->prepare("getHotelEmployees",
                                    "SELECT employee.id,firstname,secondname,lastname,phonenumber,salary, name, idhotel from employee "
                                    "INNER JOIN employeeposition ON idposition=employeeposition.id WHERE idhotel=$1;");
            }

            pqxx::work work(*connection, "getHotelEmployees");

            pqxx::result res = work.prepared("getHotelEmployees")(hotelid).exec();
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
            LOG_ERR("Failure: trying to query getHotelEmployees err: " << e.what());
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
                                    "JOIN hoteltype ON idtype=hoteltype.id ORDER BY hotel.id;");
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



ResponseCode HotelPostgresManager::countHotelRooms(uint32_t& countRooms,uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("countHotelRooms"))
            {
                connection->prepare("countHotelRooms",
                                    "Select count(*) from room where idhotel=$1;");
            }

            pqxx::work work(*connection, "countHotelRooms");

            pqxx::result res = work.prepared("countHotelRooms")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            countRooms = res[0][0].as<int>();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query countHotelRooms err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::countHotelEmployees(uint32_t &countEmployees, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("countHotelEmployees"))
            {
                connection->prepare("countHotelEmployees",
                                    "Select count(*) from employee where idhotel=$1;");
            }

            pqxx::work work(*connection, "countHotelEmployees");

            pqxx::result res = work.prepared("countHotelEmployees")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            countEmployees = res[0][0].as<int>();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query countHotelEmployees err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::countHotelOrders(uint32_t &countOrders, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("countHotelOrders"))
            {
                connection->prepare("countHotelOrders",
                                    "Select count(*) from roomorder join room on room.id=idroom where idhotel=$1;");
            }

            pqxx::work work(*connection, "countHotelOrders");

            pqxx::result res = work.prepared("countHotelOrders")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            countOrders = res[0][0].as<int>();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query countHotelOrders err: " << e.what());
            break;
        }
    }
    while(false);
}

ResponseCode HotelPostgresManager::avgResidenceTime(double &avgTime, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("avgResidenceTime"))
            {
                connection->prepare("avgResidenceTime",
                                    "SELECT AVG(EXTRACT(day from EndDate-StartDate)) FROM Room "
                                    "INNER JOIN RoomOrder ON idroom=Room.id where idhotel=$1;");
            }

            pqxx::work work(*connection, "avgResidenceTime");

            pqxx::result res = work.prepared("avgResidenceTime")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            avgTime = res[0][0].as<double>();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query avgResidenceTime err: " << e.what());
            break;
        }
    }
    while(false);
}

ResponseCode HotelPostgresManager::avgRoomRating(double &avgRating, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("avgRoomRating"))
            {
                connection->prepare("avgRoomRating",
                                    "select AVG(rating) from room where idhotel = $1;");
            }

            pqxx::work work(*connection, "avgRoomRating");

            pqxx::result res = work.prepared("avgRoomRating")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            avgRating = res[0][0].as<double>();

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query avgResidenceTime err: " << e.what());
            break;
        }
    }
    while(false);
}

ResponseCode HotelPostgresManager::getHotelRooms(std::vector<RoomInfo> &rooms, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getHotelRooms"))
            {
                connection->prepare("getHotelRooms",
                                    "SELECT room.id,places,price,rating,status,floor, roomtype.name AS type, idhotel from room "
                                    "JOIN roomtype ON idtype=roomtype.id WHERE idhotel = $1 ORDER BY room.id;");
            }

            pqxx::work work(*connection, "getHotelRooms");

            pqxx::result res = work.prepared("getHotelRooms")(hotelid).exec();
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
            LOG_ERR("Failure: trying to query getHotelRooms err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getVacantRooms(std::vector<RoomInfo> &rooms, const std::string &datebegin, const std::string &dateend, uint32_t capacity,
                                                  uint32_t startPrice, uint32_t endPrice,
                                                  uint32_t startRating, uint32_t endRating, const std::string &room_type, uint32_t hotel_id, uint32_t role)
{
    LOG_INFO(datebegin + " " + dateend + " " + std::to_string(capacity) + " " + std::to_string(startPrice) + " " + std::to_string(endPrice) + " " +
             std::to_string(startRating) + " " + std::to_string(endRating) + " " + room_type + " " + std::to_string(hotel_id));
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getVacantRooms"))
            {
                connection->prepare("getVacantRooms",
                                    "select id,places,price,rating,status,floor,idhotel, (Select name from roomtype where name = $8) as type from room "
                                    "where id not in (select distinct on(room.id) idroom from room join roomorder as ro on room.id = ro.idroom where "
                                    "to_timestamp($1::varchar,'YYYY-MM-DD') BETWEEN ro.startdate AND ro.enddate OR "
                                    "to_timestamp($2::varchar,'YYYY-MM-DD') BETWEEN ro.startdate AND ro.enddate OR "
                                    "to_timestamp($1::varchar,'YYYY-MM-DD') <= ro.startdate and "
                                    "to_timestamp($2::varchar,'YYYY-MM-DD') >= ro.enddate) "
                                    "and places=$3 and "
                                    "price BETWEEN $4 AND $5 and "
                                    "rating BETWEEN $6 and $7 and "
                                    "idtype = (Select id from roomtype WHERE name=$8::varchar) and "
                                    "idhotel = $9;");
            }

            pqxx::work work(*connection, "getVacantRooms");

            pqxx::result res = work.prepared("getVacantRooms")(datebegin)(dateend)(capacity)(startPrice)(endPrice)(startRating)(endRating)(room_type)(hotel_id).exec();
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
            LOG_ERR("Failure: trying to query getVacantRooms err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getVacantRoomsGuest(std::vector<RoomInfo> &rooms, const std::string &datebegin, const std::string &dateend, uint32_t capacity, uint32_t startPrice, uint32_t endPrice, uint32_t startRating, uint32_t endRating, const std::string &room_type, const std::string &hotel_type, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getVacantRooms"))
            {
                connection->prepare("getVacantRooms",
                                    "select id,places,price,rating,status,floor,idhotel, (Select name from roomtype where name = $8) as type from room "
                                    "where id not in (select distinct on(room.id) idroom from room join roomorder as ro on room.id = ro.idroom where "
                                    "to_timestamp($1::varchar,'YYYY-MM-DD') BETWEEN ro.startdate AND ro.enddate OR "
                                    "to_timestamp($2::varchar,'YYYY-MM-DD') BETWEEN ro.startdate AND ro.enddate OR "
                                    "to_timestamp($1::varchar,'YYYY-MM-DD') <= ro.startdate and "
                                    "to_timestamp($2::varchar,'YYYY-MM-DD') >= ro.enddate) "
                                    "and places=$3 and "
                                    "price BETWEEN $4 AND $5 and "
                                    "rating BETWEEN $6 and $7 and "
                                    "idtype = (Select id from roomtype WHERE name=$8::varchar) and "
                                    "idhotel = (Select id from hotel where name=$9::varchar);");
            }

            pqxx::work work(*connection, "getVacantRooms");

            pqxx::result res = work.prepared("getVacantRooms")(datebegin)(dateend)(capacity)(startPrice)(endPrice)(startRating)(endRating)(room_type)(hotel_type).exec();
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
            LOG_ERR("Failure: trying to query getVacantRooms err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}


ResponseCode HotelPostgresManager::getHotelOrders(std::vector<OrderInfo> &orders, uint32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getHotelOrders"))
            {
                connection->prepare("getHotelOrders",
                                    "SELECT roomorder.id, startdate, enddate, idroom,idemployee,idguest, employee.secondname,guest.secondname AS guest,idhotel "
                                    "from roomorder JOIN employee ON idemployee=employee.id JOIN guest ON guest.id=idguest WHERE idhotel = $1;");
            }

            pqxx::work work(*connection, "getHotelOrders");

            pqxx::result res = work.prepared("getHotelOrders")(hotelid).exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                OrderInfo oInfo;
                oInfo.parse_from_hn_order(value);
                orders.emplace_back(oInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getHotelOrders err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::editEmployee(int64_t user_id, const std::string fname, const std::string sname, const std::string lname,
                                                const std::string phone, int64_t salary, uint32_t position, int32_t hotelid, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("editEmployee"))
            {
                connection->prepare("editEmployee",
                                    "UPDATE employee SET firstname = $2::varchar, secondname = $3::varchar, lastname= $4::varchar,"
                                    "phonenumber = $5::varchar, salary = $6 WHERE id = $1;");
            }

            pqxx::work work(*connection, "editEmployee");

            pqxx::result res = work.prepared("editEmployee")(user_id)(fname)(sname)(lname)(phone)(salary).exec();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query editEmployee err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::editGuest(int64_t user_id, const std::string fname, const std::string sname, const std::string lname, const std::string phone, const std::string &passport, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("editGuest"))
            {
                connection->prepare("editGuest",
                                    "UPDATE guest SET firstname = $2::varchar, secondname = $3::varchar, lastname= $4::varchar,"
                                    "phonenumber = $5::varchar, passportnumber = $6::varchar WHERE id = $1;");
            }

            pqxx::work work(*connection, "editGuest");

            pqxx::result res = work.prepared("editGuest")(user_id)(fname)(sname)(lname)(phone)(passport).exec();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query editGuest err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::editHotelRoom(uint32_t id,uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("editHotelRoom"))
            {
                connection->prepare("editHotelRoom",
                                    "UPDATE room SET places = $2, price = $3, rating= $4, status = $5, floor = $6, "
                                    "idtype = (SELECT id from roomtype WHERE name = $7::varchar ), idhotel = $8 WHERE id = $1;");
            }

            pqxx::work work(*connection, "editHotelRoom");

            pqxx::result res = work.prepared("editHotelRoom")(id)(places)(price)(rating)(status)(floor)(type)(hotel_id).exec();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query editHotelRoom err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::deleteEmployee(int64_t user_id, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("deleteEmployee"))
            {
                connection->prepare("deleteEmployee",
                                    "DELETE FROM employee WHERE id = $1;");
            }

            pqxx::work work(*connection, "deleteEmployee");

            pqxx::result res = work.prepared("deleteEmployee")(user_id).exec();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query deleteEmployee err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::deleteRoom(int64_t hotel_id, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;

    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);

            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("deleteRoom"))
            {
                connection->prepare("deleteRoom",
                                    "DELETE FROM room WHERE id = $1;");
            }

            pqxx::work work(*connection, "deleteRoom");

            pqxx::result res = work.prepared("deleteRoom")(hotel_id).exec();
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query deleteRoom err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getRoomTypes(std::vector<std::string> &types, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getRoomTypes"))
            {
                connection->prepare("getRoomTypes",
                                    "SELECT name from roomtype");
            }

            pqxx::work work(*connection, "getRoomTypes");

            pqxx::result res = work.prepared("getRoomTypes").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                std::string type = value["name"].as<std::string>();
                LOG_INFO(type);
                types.emplace_back(type);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getHotelRooms err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::getHotelTypes(std::vector<HotelType> &types, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {

            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("getHotelTypes"))
            {
                connection->prepare("getHotelTypes",
                                    "SELECT * from hoteltype");
            }

            pqxx::work work(*connection, "getHotelTypes");

            pqxx::result res = work.prepared("getHotelTypes").exec();
            if(res.empty())
            {
                result = ResponseCode::status_does_not_exist;
                break;
            }
            for(const pqxx::tuple& value: res)
            {
                HotelType tInfo;
                tInfo.parse_from_hn_hoteltypes(value);
                types.emplace_back(tInfo);
            }
            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query getHotelTypes err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::addHotelRoom(uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    uint32_t statusInt = static_cast<uint32_t>(status);
    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("addHotelRoom"))
            {
                connection->prepare("addHotelRoom",
                                    "INSERT INTO room(places,price,rating,status,floor,idtype,idhotel) VALUES ($1,$2,$3,$4,$5, (SELECT roomtype.id FROM roomtype WHERE name = $6::varchar), $7);");
            }

            pqxx::work work(*connection, "addHotelRoom");

            work.prepared("addHotelRoom")(places)(price)(rating)(statusInt)(floor)(type)(hotel_id).exec();

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query addHotelRoom err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}

ResponseCode HotelPostgresManager::makeOrder(const std::string &datebegin, const std::string &dateend, uint32_t idroom, uint32_t idemployee, uint32_t idguest, uint32_t role)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        try
        {
            db_connection_ptr connection = checkConnection(role);
            if(!connection)
            {
                LOG_ERR("Cannot create connection to auth bd!");
                break;
            }

            if(!DBHelper::getDBHelper().isPrepared("makeOrder"))
            {
                connection->prepare("makeOrder",
                                    "INSERT INTO roomorder VALUES (DEFAULT,to_timestamp($1::varchar,'YYYY-MM-DD'),to_timestamp($2::varchar,'YYYY-MM-DD'),$3,NULLIF($4,0),$5);");
            }

            pqxx::work work(*connection, "makeOrder");

            work.prepared("makeOrder")(datebegin)(dateend)(idroom)(idemployee)(idguest).exec();

            work.commit();
            result = ResponseCode::status_success;
        }
        catch(const std::exception& e)
        {
            LOG_ERR("Failure: trying to query makeOrder err: " << e.what());
            break;
        }
    }
    while(false);

    return result;
}
