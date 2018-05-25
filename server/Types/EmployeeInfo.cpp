#include "EmployeeInfo.h"

void EmployeeInfo::parse_from_hn_employee(const pqxx::tuple &value)
{
    employee_id = value["id"].as<uint64_t>();
    firstN = value["firstname"].as<std::string>();
    secondN = value["secondname"].as<std::string>();
    lastN = value["lastname"].as<std::string>();
    phone  = value["phonenumber"].as<std::string>();
    salary = value["salary"].as<uint64_t>();
    position = value["name"].as<std::string>();
    hotel_id = value["idhotel"].as<uint64_t>();
}
