#ifndef SOURCE_IP_ADDRESS_H
#define SOURCE_IP_ADDRESS_H

#include "ns3/core-module.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class SourceIPAddress : public FilterElement {
public:

    SourceIPAddress() { }

    bool match(Ptr<Packet>& p) {}

private:
        Ipv4Address value;
        
};

#endif