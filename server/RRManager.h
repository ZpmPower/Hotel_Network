#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include "Server.h"
#include "ClientChannel.h"
#include "Protobuf/Message.pb.h"

class RRManager
{
public:
    RRManager(Server &server);

    void onConnected(ClientChannelPtr session);

private:
    void readSessionBuffer(std::shared_ptr<ClientChannel> session, ByteBufferPtr buffPtr);
    void disconectedSession(std::shared_ptr<ClientChannel> session);

    ResponseCode authRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode registerRR(const network::RequestContext& requests, network::ResponseContext &response);
    ResponseCode registerEmployeeRR(const network::RequestContext& requests, network::ResponseContext &response);
    ResponseCode getGuestsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getEmployeesRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getHotelsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getRoomsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getHotelRoomsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getVacantRoomsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getVacantRoomsGuestRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getHotelEmployeesRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getRoomTypesRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getHotelTypesRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getHotelOrdersRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode editEmployeeRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode editHotelRoomRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode deleteEmployeeRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode deleteRoomRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode addHotelRoomRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode makeOrderRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode getCurrentGuestsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode countHotelRoomsRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode countHotelEmployeesRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode countHotelOrdersRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode avgResTimeRR(const network::RequestContext& request, network::ResponseContext &response);
    ResponseCode avgRoomRatingRR(const network::RequestContext& request, network::ResponseContext &response);

    std::vector<ClientChannelPtr> sessions_;
};

#endif // CHATMANAGER_H
