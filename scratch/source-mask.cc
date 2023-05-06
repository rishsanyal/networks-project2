#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-address-helper.h"

using namespace ns3;
using namespace std;

class SourceMask: public FilterElement {
    private:
        Ipv4Mask value;
    public:

        SourceMask() { }

        void setValue(Ipv4Mask value){
            this->value = value;
        }

        bool match(Ptr<Packet> p) override{
            Ipv4Header header;
            p->PeekHeader(header);


            uint32_t prefixLen = value.GetPrefixLength();
            Ipv4Mask mask = Ipv4Mask((Ipv4Mask::GetOnes().Get() << (32 - prefixLen)));


            cout << "Here" << endl;
            cout << (header.GetSource().Get() && value.Get()) << endl;
            cout << "Here" << endl;

            if((header.GetSource().Get() & mask.Get()) == (value.Get() & mask.Get())){
                return true;
            } else {
                return false;
            }

            return false;
        }
};