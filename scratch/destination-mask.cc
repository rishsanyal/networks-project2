#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"

using namespace ns3;
using namespace std;

class DestinationMask: public FilterElement {
    private:
        Ipv4Mask value;    
    public:
            
        DestinationMask() { }

        bool match(Ptr<Packet>& p) {
            Ipv4Header header;
            p->PeekHeader(header);

            uint32_t prefixLen = value.GetPrefixLength();
            Ipv4Mask mask = Ipv4Mask((Ipv4Mask::GetOnes().Get() << (32 - prefixLen)));

            if((header.GetDestination().Get() & mask.Get()) == (value.Get() & mask.Get())){
                return true;
            } else {
                return false;
            }
        }
};