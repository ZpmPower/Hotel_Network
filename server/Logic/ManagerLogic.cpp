#include "ManagerLogic.h"
#include "db/HotelPostgresManager.h"
#include "Types/GuestInfo.h"
#include "Types/EmployeeInfo.h"
#include "Types/HotelInfo.h"
#include "Types/RoomInfo.h"
#include "db/SessionManagerPostgres.h"

ResponseCode ManagerLogic::getHotelEmployees(network::EmployeesMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        std::vector<EmployeeInfo> employees;
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        result = HotelPostgresManager::getHotelEmployees(employees,request.hotel_id().hotelid(),role);
        for(EmployeeInfo info: employees)
        {

            network::EmployeeInfo* employee = responce->add_employees();
            employee->set_firstname(info.firstN);
            employee->set_secondname(info.secondN);
            employee->set_lastname(info.lastN);
            employee->set_phonenumber(info.phone);
            employee->set_salary(info.salary);
            employee->set_position(info.position);
            employee->set_id(info.employee_id);
            employee->set_hotelid(info.hotel_id);
        }
    }
     while(false);
     return result;
}

ResponseCode ManagerLogic::editEmployee(network::RegisterMessageResponse *responce, const network::RequestContext &request)
{
    ResponseCode result = ResponseCode::status_internal_error;
    do
    {
        uint32_t role;
        SessionManagerPostgres::getRoleBySession(request.session_info().session_id(),role);
        network::EmployeeInfo info = request.employee_info();
        uint32_t position = positionToInt(info.position());

        result = HotelPostgresManager::editEmployee(info.id(),info.firstname(),info.secondname(),info.lastname(),info.phonenumber(),info.salary(),position,info.hotelid(),role);
        responce->set_messagetext("Success");
    }
     while(false);
     return result;
}

uint32_t ManagerLogic::positionToInt(const std::string &pos)
{
    if(pos=="Manager") return 1;
    if(pos=="Receptionist") return 2;
    if(pos=="Admin") return 0;
    if(pos=="Guest") return 3;
}
