#include "../headers/packet.h"

Packet::Packet(string _string="", PacketType _type)
{
    msg = _string;
    type = _type;
}

void Packet::setString(const string &newString)
{
    msg = _string;
}

string Packet::getString()
{
    return msg;
}

PacketType Packet::getType()
{
    return type;
}