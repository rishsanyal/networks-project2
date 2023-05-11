#ifndef DESTINTATION_MASK_H
#define DESTINTATION_MASK_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

namespace ns3{

class DestinationIPMask: public FilterElement {
    private:
        Ipv4Mask value;
        Ipv4Address address;

    public:
        DestinationIPMask(){ }
        void setValue(Ipv4Address address, Ipv4Mask value);
        bool match(Ptr<Packet> p) override;
};

DestinationIPMask::DestinationIPMask() {}


void DestinationIPMask::setValue(Ipv4Address address, Ipv4Mask value){
    this->value = value;
    this->address = address;
}

bool DestinationIPMask::match(Ptr<Packet> p){

    Ptr<ns3::Packet> tempPacket = p->Copy();

    PppHeader pppHeader;
    tempPacket->RemoveHeader(pppHeader);

    Ipv4Header ipHeader;
    tempPacket->RemoveHeader(ipHeader);
    
    UdpHeader udpHeader;
    tempPacket->RemoveHeader(udpHeader);

    if (this->value.IsMatch(ipHeader.GetDestination(), this->address)){
        return true;
    }

    return false;
}

}

#endif