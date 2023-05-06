#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"

using namespace ns3;
using namespace std;

class ProtocolNumber : public FilterElement {
    private:
        uint32_t value;
    public:
        ProtocolNumber() {}

        void setValue(uint32_t value){
            this->value = value;
        }

        bool match(Ptr<Packet> p) override{
            Ipv4Header header;
            p->PeekHeader(header);

            if(header.GetProtocol() == value){
                // 1
                return true;
            }

            return false;
        }
};