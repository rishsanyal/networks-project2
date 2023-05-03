#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"
#include "ns3/udp-header.h"

using namespace ns3;
using namespace std;

class DestinationPortNumber: public FilterElement {
    private:
        uint32_t value;
    public:
    
        DestinationPortNumber() { }

        bool match(Ptr<Packet>& p) {
            UdpHeader udpHeader;
            p->PeekHeader(udpHeader);
            if(udpHeader.GetDestinationPort() == value) {
                return true;
            } else {
                return false;
            }
        }
};