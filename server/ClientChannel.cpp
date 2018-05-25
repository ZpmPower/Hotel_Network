#include "ClientChannel.h"
#include <memory>
#include "Helper.h"

ClientChannel::ClientChannel(asio::ssl::context& context)
    :Session(context)
{

}

std::shared_ptr<ClientChannel> ClientChannel::getNewSessions(asio::ssl::context& context)
{
    std::shared_ptr<ClientChannel> sessionPtr = std::make_shared<ClientChannel>(context);
    return sessionPtr;
}

void ClientChannel::onRead(ByteBuffer buffer)
{
    if(nullptr != readHandle_)
    {
        ByteBufferPtr buff = std::make_shared<ByteBuffer>(std::move(buffer));
        readHandle_(getPtrFromThis(), buff);
    }
}

void ClientChannel::onDisconected()
{
    disconectedHandle_(getPtrFromThis());
}

std::shared_ptr<ClientChannel> ClientChannel::getPtrFromThis()
{
    return std::static_pointer_cast<ClientChannel>(shared_from_this());
}

void ClientChannel::setDisconectedHandle(const std::function<void (std::shared_ptr<ClientChannel>)> &disconectedHandle)
{
    disconectedHandle_ = disconectedHandle;
}

void ClientChannel::setReadHandle(const std::function<void (std::shared_ptr<ClientChannel>, ByteBufferPtr)> &value)
{
    readHandle_ = value;
}

void ClientChannel::execute(ByteBufferPtr data)
{
    write(data);
}

void ClientChannel::execute(const std::string &data)
{
    LOG_INFO(data);
    ByteBufferPtr buff = std::make_shared<ByteBuffer>(data.begin(), data.end());
    execute(buff);
}
