#ifndef SOURCE_MASK_H
#define SOURCE_MASK_H

#include "ns3/core-module.h"
#include "ns3/ipv4-address.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

namespace ns3{

class SourceIPMask : public FilterElement {
    private:
        Ipv4Mask value;
        Ipv4Address address;

    public:
        SourceIPMask();
        void setValue(Ipv4Address address, Ipv4Mask value);
        bool match(Ptr<Packet> p) override;
};

SourceIPMask::SourceIPMask() {}


void SourceIPMask::setValue(Ipv4Address address, Ipv4Mask value){
    this->value = value;
    this->address = address;
}

bool SourceIPMask::match(Ptr<Packet> p){

    Ptr<ns3::Packet> tempPacket = p->Copy();

    PppHeader pppHeader;
    tempPacket->RemoveHeader(pppHeader);

    Ipv4Header ipHeader;
    tempPacket->RemoveHeader(ipHeader);
    
    UdpHeader udpHeader;
    tempPacket->RemoveHeader(udpHeader);

    if (this->value.IsMatch(ipHeader.GetSource(), this->address)){
        return true;
    }

    return false;
}

}

#endif