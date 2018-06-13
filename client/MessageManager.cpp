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
    context.set_allocated_session_info(session);


    employeeData->set_firstname(fname);
    employeeData->set_secondname(sname);
    employeeData->set_lastname(lname);
    employeeData->set_phonenumber(phone);
    employeeData->set_salary(salary);
    employeeData->set_position(position);
    employeeData->set_hotelid(hotelid);
    employeeData->set_id(employeeId);

    context.set_allocated_employee_info(employeeData);

    execute(context.SerializeAsString());
}

bool MessageManager::editGuest(uint32_t guestId, const std::string &fname, const std::string &sname, const std::string &lname, const std::string &phone, const std::string &passport)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_EDIT_GUEST);

    network::GuestInfo* guestData = new network::GuestInfo();
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());
    context.set_allocated_session_info(session);

    guestData->set_firstname(fname);
    guestData->set_secondname(sname);
    guestData->set_lastname(lname);
    guestData->set_phonenumber(phone);
    guestData->set_passport(passport);
    guestData->set_id(guestId);

    context.set_allocated_guest_info(guestData);

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

void MessageManager::getGuestbyId(uint32_t id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_GUEST_BY_ID);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(id);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::countHotelRooms(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_COUNT_HOTEL_ROOMS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::countHotelEmployees(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_COUNT_HOTEL_EMPLOYEES);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::countHotelOrders(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_COUNT_HOTEL_ORDERS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::avgResidenceTime(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_AVG_RESIDENCE_TIME);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::avgRoomRating(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_AVG_ROOM_RATING);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::getHotelRooms(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_HOTEL_ROOMS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::getHotelOrders(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_HOTEL_ORDERS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::getHotelFloors(uint32_t hotelid)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_HOTEL_FLOORS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotelid);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::getGuestsOrders(uint32_t guest_id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_GUEST_ALL_ORDERS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* sessionInfo = new network::SessionInfo();
    sessionInfo->set_session_id(getSession().session_id());
    hotelId->set_hotelid(guest_id);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(sessionInfo);
    execute(context.SerializeAsString());
}

void MessageManager::deleteEmployee(uint32_t employeeId)
{
    network::RequestContext context;
    network::EmployeeInfo* employeeData = new network::EmployeeInfo();
    network::SessionInfo* session = new network::SessionInfo();
    employeeData->set_id(employeeId);
    session->set_session_id(getSession().session_id());


    context.set_message_type_(network::message_type::HN_DELETE_EMPLOYEE);
    context.set_allocated_session_info(session);
    context.set_allocated_employee_info(employeeData);
    execute(context.SerializeAsString());
}

void MessageManager::deleteRoom(uint32_t roomId)
{
    network::RequestContext context;
    network::RoomInfo* roomData = new network::RoomInfo();
    network::SessionInfo* session = new network::SessionInfo();
    roomData->set_id(roomId);
    session->set_session_id(getSession().session_id());   
    context.set_allocated_session_info(session);

    context.set_message_type_(network::message_type::HN_DELETE_ROOM);
    context.set_allocated_room_info(roomData);
    execute(context.SerializeAsString());
}

void MessageManager::getRoomTypes()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_ROOM_TYPES);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());
    context.set_allocated_session_info(session);
    execute(context.SerializeAsString());
}

void MessageManager::getHotelTypes()
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_HOTEL_TYPES);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());
    context.set_allocated_session_info(session);
    execute(context.SerializeAsString());
}

void MessageManager::getCurrentGuests(uint32_t hotel_id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_CURRENT_GUESTS);
    network::HotelId* hotelId = new network::HotelId();
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());
    hotelId->set_hotelid(hotel_id);
    context.set_allocated_hotel_id(hotelId);
    context.set_allocated_session_info(session);
    execute(context.SerializeAsString());
}

void MessageManager::getVacantRooms(const std::string &datebegin, const std::string &dateend, uint32_t capacity, uint32_t startPrice, uint32_t endPrice,
                                    uint32_t startRating, uint32_t endRating, const std::string &room_type, uint32_t hotel_id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_VACANT_ROOMS);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());

    network::VacantRooms* info = new network::VacantRooms();
    info->set_datebegin(datebegin);
    info->set_dateend(dateend);
    info->set_places(capacity);
    info->set_begin_price(startPrice);
    info->set_end_price(endPrice);
    info->set_begin_rating(startRating);
    info->set_end_rating(endRating);
    info->set_room_type(room_type);
    info->set_hotelid(hotel_id);

    context.set_allocated_data(info);
    context.set_allocated_session_info(session);

    execute(context.SerializeAsString());
}

void MessageManager::getVacantRoomsGuest(const std::string &datebegin, const std::string &dateend, uint32_t capacity, uint32_t startPrice, uint32_t endPrice, uint32_t startRating, uint32_t endRating, const std::string &room_type, const std::string &hotel_name)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_GET_VACANT_ROOMS_GUEST);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());

    network::VacantRoomsGuest* info = new network::VacantRoomsGuest();
    info->set_datebegin(datebegin);
    info->set_dateend(dateend);
    info->set_places(capacity);
    info->set_begin_price(startPrice);
    info->set_end_price(endPrice);
    info->set_begin_rating(startRating);
    info->set_end_rating(endRating);
    info->set_room_type(room_type);
    info->set_hotel_type(hotel_name);

    context.set_allocated_vacant_rooms_guest(info);
    context.set_allocated_session_info(session);

    execute(context.SerializeAsString());
}

void MessageManager::makeOrder(const std::string &datebegin, const std::string &dateend, uint32_t idroom, uint32_t idemployee, uint32_t idguest)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_MAKE_ORDER);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());


    network::MakeOrderInfo* makeOrderInfo = new network::MakeOrderInfo();
    makeOrderInfo->set_startdate(datebegin);
    makeOrderInfo->set_enddate(dateend);
    makeOrderInfo->set_idemployee(idemployee);
    makeOrderInfo->set_idguest(idguest);
    makeOrderInfo->set_idroom(idroom);


    context.set_allocated_make_order_info(makeOrderInfo);
    context.set_allocated_session_info(session);

    execute(context.SerializeAsString());
}


void MessageManager::addRoom(uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_ADD_HOTEL_ROOM);
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());


    network::RoomInfo* room = new network::RoomInfo();
    room->set_places(places);
    room->set_price(price);
    room->set_rating(rating);
    room->set_status(status);
    room->set_floor(floor);
    room->set_type(type);
    room->set_hotelid(hotel_id);


    context.set_allocated_room_info(room);
    context.set_allocated_session_info(session);

    execute(context.SerializeAsString());
}

void MessageManager::editRoom(uint32_t id, uint32_t places, uint32_t price, uint32_t rating, bool status, uint32_t floor, const std::string &type, uint32_t hotel_id)
{
    network::RequestContext context;
    context.set_message_type_(network::message_type::HN_EDIT_HOTEL_ROOM);

    network::RoomInfo* roomData = new network::RoomInfo();
    network::SessionInfo* session = new network::SessionInfo();
    session->set_session_id(getSession().session_id());
    context.set_allocated_session_info(session);


    roomData->set_places(places);
    roomData->set_price(price);
    roomData->set_rating(rating);
    roomData->set_status(status);
    roomData->set_floor(floor);
    roomData->set_type(type);
    roomData->set_hotelid(hotel_id);
    roomData->set_id(id);

    context.set_allocated_room_info(roomData);

    execute(context.SerializeAsString());
}
