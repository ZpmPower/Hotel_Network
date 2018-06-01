#include "MessageManager.h"
#include "Worker.h"
#include <string>
#include "Protobuf/Message.pb.h"
#include "Crypto/CryptoHelper.h"

MessageManager::MessageManager(const std::string &address, const std::string &port)
    : context_(asio::ssl::context::sslv23)
{
    context_.load_verify_file("server.crt");
    clientChatPtr_ = std::make_shared<RRMananger>(address, port, context_);
}

void MessageManager::start()
{
    clientChatPtr_->start();
    Worker::instance()->start();
    Worker::instance()->join();
}

void MessageManager::setOnErrorCB(const std::function<void (ClientError)> &onError)
{
    if(clientChatPtr_)
    {
        clientChatPtr_->setOnErrorCB(onError);
    }
}

void MessageManager::setOnReadCB(const std::function<void (const network::ResponseContext &)> &onRead)
{
    if(clientChatPtr_)
    {
        clientChatPtr_->setOnRead(onRead);
    }
}

void MessageManager::execute(ByteBufferPtr buff)
{
    if(clientChatPtr_ && buff)
    {
        clientChatPtr_->execute(buff);
    }
}

void MessageManager::execute(const std::string &buff)
{
    ByteBufferPtr buffPtr = std::make_shared<ByteBuffer>(buff.begin(), buff.end());

    execute(buffPtr);
}

bool MessageManager::userAuth(const std::string &login, const std::string &password)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_AUTH);

    network::AuthMessage* authMess = new network::AuthMessage();
    authMess->set_login(login);

    std::string hashedPass;
    CryptoHelper::md5_hash(password, hashedPass);
    authMess->set_pass(hashedPass);
    context.set_allocated_auth_message_(authMess);
    LOG_INFO(hashedPass);
    execute(context.SerializeAsString());
}

bool MessageManager::createGuest(const std::string &login, const std::string &password, const std::string &fname, const std::string &sname, const std::string &lname,
                                const std::string &phone, const std::string &passport, uint32_t role)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_REGISTER);

    network::RegisterMessage* regMess = new network::RegisterMessage();
    regMess->set_login(login);
    std::string hashPass;
    CryptoHelper::md5_hash(password, hashPass);
    regMess->set_pass(hashPass);
    regMess->set_firstname(fname);
    regMess->set_secondname(sname);
    regMess->set_lastname(lname);
    regMess->set_phonenumber(phone);
    regMess->set_passport(passport);
    regMess->set_role(role);
    context.set_allocated_register_message_(regMess);

    execute(context.SerializeAsString());
}

bool MessageManager::createEmployee(const std::string &login, const std::string &password, const std::string &fname, const std::string &sname, const std::string &lname,
                                    const std::string &phone, int64_t salary, int32_t position, int32_t hotelid, uint32_t role)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_REGISTER_EMPLOYEE);

    network::RegisterEmployeeMessage* regMess = new network::RegisterEmployeeMessage();
    regMess->set_login(login);
    std::string hashPass;
    CryptoHelper::md5_hash(password, hashPass);
    regMess->set_pass(hashPass);
    regMess->set_firstname(fname);
    regMess->set_secondname(sname);
    regMess->set_lastname(lname);
    regMess->set_phonenumber(phone);
    regMess->set_salary(salary);
    regMess->set_position(position);
    regMess->set_hotelid(hotelid);
    regMess->set_role(role);

    context.set_allocated_register_employee_message_(regMess);

    execute(context.SerializeAsString());
}

bool MessageManager::editEmployee(uint32_t employeeId, const std::string &fname, const std::string &sname, const std::string &lname, const std::string &phone,
                                  int64_t salary, const std::string& position, int32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_EDIT_EMPLOYEE);

    network::EmployeeInfo* employeeData = new network::EmployeeInfo();
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());

    employeeData->set_firstname(fname);
    employeeData->set_secondname(sname);
    employeeData->set_lastname(lname);
    employeeData->set_phonenumber(phone);
    employeeData->set_salary(salary);
    employeeData->set_position(position);
    employeeData->set_hotelid(hotelid);
    employeeData->set_id(employeeId);

    context.set_allocated_session_info(session);
    context.set_allocated_employee_info(employeeData);

    execute(context.SerializeAsString());
}

void MessageManager::getGuests()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ALL_GUESTS);
    execute(context.SerializeAsString());
}

void MessageManager::getEmployees()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ALL_EMPLOYEES);
    execute(context.SerializeAsString());
}

void MessageManager::getHotelEmployees(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_HOTEL_EMPLOYEES);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::getHotels()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ALL_HOTELS);
    execute(context.SerializeAsString());
}

void MessageManager::getRooms()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ALL_ROOMS);
    execute(context.SerializeAsString());
}

void MessageManager::deleteEmployee(uint32_t employeeId)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ALL_ROOMS);
    execute(context.SerializeAsString());
}