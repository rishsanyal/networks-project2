#ifndef DESTINATION_IP_ADDRESS_H
#define DESTINATION_IP_ADDRESS_H

#include "ns3/core-module.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

namespace ns3 {
class DestinationIPAddress : public FilterElement {
    public:
        DestinationIPAddress();
        void setValue(Ipv4Address value);
        bool match(Ptr<Packet> p) override;

    private:
        Ipv4Address value;

};

DestinationIPAddress::DestinationIPAddress() {}

void DestinationIPAddress::setValue(ns3::Ipv4Address value){
    this->value = value;
}

bool DestinationIPAddress::match(Ptr<Packet> p){

    Ptr<ns3::Packet> tempPacket = p->Copy();

    PppHeader pppHeader;
    tempPacket->RemoveHeader(pppHeader);

    Ipv4Header ipHeader;
    tempPacket->RemoveHeader(ipHeader);

    UdpHeader udpHeader;
    tempPacket->RemoveHeader(udpHeader);

    if(ipHeader.GetDestination()==value){
        return true;
    } else {
        cout << ipHeader.GetDestination() << endl;
        cout << "DestinationIPAddress: " << this->value << endl;
        return false;
    }
}

};

#endif