#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/udp-header.h"

using namespace ns3;
using namespace std;

class SourceIPMask: public FilterElement {
    private:
        Ipv4Mask value;
        Ipv4Address address;
    public:

        SourceIPMask() { }

        void setValue(Ipv4Address address, Ipv4Mask value){
            this->value = value;
            this->address = address;
        }

        bool match(Ptr<Packet> p) override{
            Ptr<Packet> copy = p->Copy();

            Ipv4Header header;
            copy->RemoveHeader(header);

            UdpHeader udpHeader;
            copy->RemoveHeader(udpHeader);

            if (this->value.IsMatch(header.GetSource(), this->address)){
                return true;
            }

            return false;
        }
};