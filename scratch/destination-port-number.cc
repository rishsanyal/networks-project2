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

        DestinationPortNumber() {}

        void setValue(uint32_t value){
            this->value = value;
        }

        bool match(Ptr<Packet> p) {
            Ipv4Header header;
            p->PeekHeader(header);

            UdpHeader udpHeader;
            p->PeekHeader(udpHeader);

            Ptr<Packet> C_Packet = p->Copy();
            C_Packet->RemoveHeader(udpHeader);

            if(udpHeader.GetDestinationPort() == value){
                return true;
            }

            return false;
        }
};