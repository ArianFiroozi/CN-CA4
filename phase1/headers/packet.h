#ifndef PACKET_H
#define PACKET_H

#include <string>

enum PacketType{ACK, SYN, SYN_ACK};

class Packet
{
private:
    PacketType type;
    string msg;

public:
    Packet(string _string="", PacketType _type);

    void setString(const string &newString);
    string getString() const;
    PacketType getType() const;
};

#endif // PACKET_H