#ifndef EMPLOYEEINFO_H
#define EMPLOYEEINFO_H

#include "define.h"
#include "pqxx/tuple"

struct EmployeeInfo
{
    uint64_t employee_id;
    std::string firstN;
    std::string secondN;
    std::string lastN;
    std::string phone;
    uint64_t salary;
    std::string position;
    uint64_t hotel_id;

    void parse_from_hn_employee(const pqxx::tuple& value);
};

#endif // EMPLOYEEINFO_H
