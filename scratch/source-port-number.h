#ifndef SOURCE_PORTNUMBER_H
#define SOURCE_PORTNUMBER_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"
#include "ns3/udp-header.h"
#include "ns3/ppp-header.h"
#include "ns3/ipv4-header.h"

using namespace ns3;
using namespace std;

namespace ns3 {
class SourcePortNumber : public FilterElement {
    public:
        SourcePortNumber();
        void setValue(uint32_t value);
        bool match(Ptr<Packet> p) override;

    private:
            uint32_t value;

};

SourcePortNumber::SourcePortNumber() {}

void
SourcePortNumber::setValue(uint32_t value){
    this->value = value;
}

bool
SourcePortNumber::match(Ptr<Packet> p) {

    Ptr<ns3::Packet> tempPacket = p->Copy();

    PppHeader pppHeader;
    tempPacket->RemoveHeader(pppHeader);

    Ipv4Header ipHeader;
    tempPacket->RemoveHeader(ipHeader);

    UdpHeader udpHeader;
    tempPacket->RemoveHeader(udpHeader);

    if(udpHeader.GetSourcePort() == value){
        return true;
    }

    return false;
}

}

#endif