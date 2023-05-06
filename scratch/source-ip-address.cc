#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"

using namespace ns3;
using namespace std;

class SourceIPAddress : public FilterElement {
    private:
        Ipv4Address value;
    public:

        SourceIPAddress() {
         }

        void setValue(Ipv4Address value = Ipv4Address::GetAny()){
            this->value = value;
        }

        bool match(Ptr<Packet> p) override{
            Ipv4Header header;
            p->PeekHeader(header);

            if(header.GetSource() == value){
                // 1
                return true;
            } else {
                // 0
                return false;
            }
        }
};