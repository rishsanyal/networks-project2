#include "ns3/core-module.h"
#include "filter-element.h"
#include "ns3/ipv4-header.h"

using namespace ns3;
using namespace std;

class SourceIPAddress : public FilterElement {
    private:
        Ipv4Address value;
    public:
            
        SourceIPAddress() { }

        bool match(Ptr<Packet>& p) {
            Ipv4Header header;
            p->PeekHeader(header);

            if(header.GetSource()==value){
                return true;
            } else {
                return false;
            }
        }
};