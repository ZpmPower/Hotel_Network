#include "RRManager.h"
#include "Helper.h"
#include "Logic/AuthLogic.h"
#include "Logic/AdminLogic.h"
#include "Logic/ManagerLogic.h"

RRManager::RRManager(Server& server)
{
    server.subscribe(std::bind(
                         &RRManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void RRManager::onConnected(ClientChannelPtr session)
{
    LOG_INFO("Connected session");
    sessions_.emplace_back(session);

    session->setReadHandle(std::bind(&RRManager::readSessionBuffer
                                     , this
                                     , std::placeholders::_1
                                     , std::placeholders::_2));

    session->setDisconectedHandle(std::bind(&RRManager::disconectedSession
                                            , this
                                            , std::placeholders::_1));
}
// /////////////////////////////////////
void RRManager::readSessionBuffer(std::shared_ptr<ClientChannel> session, ByteBufferPtr buffPtr)
{
    network::RequestContext reqContext;
    network::ResponseContext resContext;
    reqContext.ParseFromString(Helper::bufferToString(buffPtr, 0, 0));

    ResponseCode responseCode;
    switch (reqContext.message_type_()) {
    case network::HN_AUTH:
        responseCode = authRR(reqContext, resContext);
        break;
    case network::HN_REGISTER:
        responseCode = registerRR(reqContext, resContext);
        break;
    case network::HN_REGISTER_EMPLOYEE:
        responseCode = registerEmployeeRR(reqContext,resContext);
        break;
    case network::HN_GET_ALL_GUESTS:
        responseCode = getGuestsRR(reqContext, resContext);
        break;
    case network::HN_GET_ALL_EMPLOYEES:
        responseCode = getEmployeesRR(reqContext, resContext);
        break;
    case network::HN_GET_ALL_HOTELS:
        responseCode = getHotelsRR(reqContext, resContext);
        break;
    case network::HN_GET_ALL_ROOMS:
        responseCode = getRoomsRR(reqContext, resContext);
        break;
    case network::HN_GET_HOTEL_ROOMS:
        responseCode = getHotelRoomsRR(reqContext, resContext);
        break;
    case network::HN_GET_HOTEL_EMPLOYEES:
        responseCode = getHotelEmployeesRR(reqContext, resContext);
        break;
    case network::HN_EDIT_EMPLOYEE:
        responseCode = editEmployeeRR(reqContext, resContext);
        break;
    case network::HN_DELETE_EMPLOYEE:
        responseCode = deleteEmployeeRR(reqContext, resContext);
        break;
    case network::HN_DELETE_ROOM:
        responseCode = deleteRoomRR(reqContext, resContext);
        break;
    case network::HN_GET_ROOM_TYPES:
        responseCode = getRoomTypesRR(reqContext, resContext);
        break;
    case network::HN_ADD_HOTEL_ROOM:
        responseCode = addHotelRoomRR(reqContext, resContext);
        break;
    case network::HN_EDIT_HOTEL_ROOM:
        responseCode = editHotelRoomRR(reqContext, resContext);
        break;
    case network::HN_GET_HOTEL_ORDERS:
        responseCode = getHotelOrdersRR(reqContext, resContext);
        break;
    case network::HN_GET_HOTEL_TYPES:
        responseCode = getHotelTypesRR(reqContext, resContext);
        break;
    case network::HN_GET_VACANT_ROOMS:
        responseCode = getVacantRoomsRR(reqContext, resContext);
        break;
    case network::HN_GET_VACANT_ROOMS_GUEST:
        responseCode = getVacantRoomsGuestRR(reqContext, resContext);
        break;
    case network::HN_MAKE_ORDER:
        responseCode = makeOrderRR(reqContext, resContext);
        break;
    case network::HN_GET_CURRENT_GUESTS:
        responseCode = getCurrentGuestsRR(reqContext, resContext);
        break;
    case network::HN_COUNT_HOTEL_ROOMS:
        responseCode = countHotelRoomsRR(reqContext, resContext);
        break;
    case network::HN_COUNT_HOTEL_EMPLOYEES:
        responseCode = countHotelEmployeesRR(reqContext, resContext);
        break;
    case network::HN_COUNT_HOTEL_ORDERS:
        responseCode = countHotelOrdersRR(reqContext, resContext);
        break;
    case network::HN_AVG_RESIDENCE_TIME:
        responseCode = avgResTimeRR(reqContext, resContext);
        break;
    case network::HN_AVG_ROOM_RATING:
        responseCode = avgRoomRatingRR(reqContext, resContext);
        break;
    default:
        responseCode = ResponseCode::status_unknown_command;
        break;
    }

    resContext.set_error_code(static_cast<int32_t>(responseCode));
    resContext.set_message_type_(reqContext.message_type_());
    session->execute(resContext.SerializeAsString());
}
// /////////////////////////////
void RRManager::disconectedSession(std::shared_ptr<ClientChannel> session)
{
    LOG_INFO("Disconected!");
}

ResponseCode RRManager::authRR(const network::RequestContext &request, network::ResponseContext& response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        if(!request.has_auth_message_())
        {
            LOG_ERR("Where is not auth message!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        network::AuthMessage authMessage = request.auth_message_();

        if(!authMessage.has_login())
        {
            LOG_ERR("Where is not login value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        if(!authMessage.has_pass())
        {
            LOG_ERR("Where is not password value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        network::AuthMessageResponse* authRes = new network::AuthMessageResponse();
        network::SessionInfo* sessionInfo = new network::SessionInfo();

        resultStatus = AuthLogic::authUser(authMessage, authRes,sessionInfo);

        response.set_allocated_session_info(sessionInfo);
        response.set_allocated_auth_response(authRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::registerRR(const network::RequestContext &requests, network::ResponseContext& response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        if(!requests.has_register_message_())
        {
            LOG_ERR("Where is not register message!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        network::RegisterMessage authMessage = requests.register_message_();

        if(!authMessage.has_login())
        {
            LOG_ERR("Where is not login value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        if(!authMessage.has_pass())
        {
            LOG_ERR("Where is not password value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = AuthLogic::createUser(authMessage, regRes);

        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::registerEmployeeRR(const network::RequestContext &requests, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        if(!requests.has_register_employee_message_())
        {
            LOG_ERR("Where is not register message!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        network::RegisterEmployeeMessage authMessage = requests.register_employee_message_();

        if(!authMessage.has_login())
        {
            LOG_ERR("Where is not login value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }

        if(!authMessage.has_pass())
        {
            LOG_ERR("Where is not password value!");
            resultStatus = ResponseCode::status_bad_request;
            break;
        }
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = AuthLogic::createEmployee(authMessage, regRes);

        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getGuestsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::GuestsMessageResponse* guestsRes = new network::GuestsMessageResponse();

        resultStatus = AdminLogic::getGuests(guestsRes);
        response.set_allocated_guests(guestsRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getEmployeesRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::EmployeesMessageResponse* employeesRes = new network::EmployeesMessageResponse();

        resultStatus = AdminLogic::getEmployees(employeesRes);
        response.set_allocated_employees(employeesRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getHotelEmployeesRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::EmployeesMessageResponse* employeesRes = new network::EmployeesMessageResponse();

        resultStatus = ManagerLogic::getHotelEmployees(employeesRes,request);
        response.set_allocated_employees(employeesRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getRoomTypesRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RoomTypesMessageResponse* typesRes = new network::RoomTypesMessageResponse();

        resultStatus = ManagerLogic::getRoomTypes(typesRes,request);
        response.set_allocated_types(typesRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getHotelTypesRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::HotelTypesMessageResponse* typesRes = new network::HotelTypesMessageResponse();

        resultStatus = ManagerLogic::getHotelTypes(typesRes,request);
        response.set_allocated_hotel_types(typesRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getHotelOrdersRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::OrdersMessageResponse* ordersRes = new network::OrdersMessageResponse();

        resultStatus = ManagerLogic::getHotelOrders(ordersRes,request);
        response.set_allocated_orders(ordersRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::editEmployeeRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;
    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::editEmployee(regRes,request);
        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::editHotelRoomRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;
    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::editHotelRoom(regRes,request);
        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::deleteEmployeeRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;
    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::deleteEmployee(regRes,request);
        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::deleteRoomRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;
    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::deleteRoom(regRes,request);
        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::addHotelRoomRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::addHotelRoom(regRes,request);

        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::makeOrderRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* regRes = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::makeOrder(regRes,request);

        response.set_allocated_register_response(regRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getCurrentGuestsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::GuestOrdersMessageResponse* guestRes = new network::GuestOrdersMessageResponse();

        resultStatus = ManagerLogic::getCurrentGuestsRR(guestRes,request);
        response.set_allocated_guest_orders(guestRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::countHotelRoomsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* countRoom = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::countHotelRooms(countRoom,request);
        response.set_allocated_register_response(countRoom);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::countHotelEmployeesRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* countEmployees = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::countHotelEmployeess(countEmployees,request);
        response.set_allocated_register_response(countEmployees);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::countHotelOrdersRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* countOrders = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::countHotelOrders(countOrders,request);
        response.set_allocated_register_response(countOrders);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::avgResTimeRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* avgTime = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::avgResidenceTime(avgTime,request);
        response.set_allocated_register_response(avgTime);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::avgRoomRatingRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RegisterMessageResponse* avgRating = new network::RegisterMessageResponse();

        resultStatus = ManagerLogic::avgRoomRating(avgRating,request);
        response.set_allocated_register_response(avgRating);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getHotelsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::HotelsMessageResponse* hotelsRes = new network::HotelsMessageResponse();

        resultStatus = AdminLogic::getHotels(hotelsRes);
        response.set_allocated_hotels(hotelsRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getRoomsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RoomsMessageResponse* roomsRes = new network::RoomsMessageResponse();

        resultStatus = AdminLogic::getRooms(roomsRes);
        response.set_allocated_rooms(roomsRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getHotelRoomsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RoomsMessageResponse* roomsRes = new network::RoomsMessageResponse();

        resultStatus = ManagerLogic::getHotelRooms(roomsRes,request);
        response.set_allocated_rooms(roomsRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getVacantRoomsRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RoomsMessageResponse* roomsRes = new network::RoomsMessageResponse();

        resultStatus = ManagerLogic::getVacantRooms(roomsRes,request);
        response.set_allocated_rooms(roomsRes);
    }
    while(false);

    return resultStatus;
}

ResponseCode RRManager::getVacantRoomsGuestRR(const network::RequestContext &request, network::ResponseContext &response)
{
    ResponseCode resultStatus = ResponseCode::status_internal_error;

    do
    {
        network::RoomsMessageResponse* roomsRes = new network::RoomsMessageResponse();

        resultStatus = ManagerLogic::getVacantRoomsGuest(roomsRes,request);
        response.set_allocated_rooms(roomsRes);
    }
    while(false);

    return resultStatus;
}


