#include "Helper.h"


ByteBufferPtr Helper::makeBuffer(ByteBufferPtr buffPtr)
{
    uint16_t msgSize = buffPtr->size();

    uint16_t lowByte = msgSize & 0x00FF;
    uint16_t highByte = msgSize >> 8;

    buffPtr->emplace(buffPtr->begin(), static_cast<char>(lowByte));
    buffPtr->emplace(buffPtr->begin(), static_cast<char>(highByte));

    return buffPtr;
}

uint16_t Helper::mergeTwoByte(uint8_t high, uint8_t low)
{
    uint16_t number = (high << 8) | low;
    return number;
}

ByteBuffer Helper::stringToBuffer(const std::string &str)
{
    return ByteBuffer(str.begin(), str.end());
}

std::string Helper::bufferToString(ByteBufferPtr buffPtr, uint posFrom, uint posTo)
{
    return std::string(buffPtr->begin()+posFrom, buffPtr->end() - posTo);
}

Roles Helper::roleToInt(const std::string &role)
{
    if(role == "Manager")
    {
        return Roles::role_manager;
    }
    if(role == "Receptionist")
    {
        return Roles::role_receptionist;
    }
    if(role == "Admin")
    {
        return Roles::role_admin;
    }
    if(role == "Guest")
    {
        return Roles::role_guest;
    }
}
