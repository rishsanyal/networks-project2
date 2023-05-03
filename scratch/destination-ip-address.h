
#ifndef DESTINATION_IP_ADDRESS_H
#define DESTINATION_IP_ADDRESS_H

#include "ns3/core-module.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class DestinationIPAddress : public FilterElement {
public:

    DestinationIPAddress() { }

    bool match(Ptr<Packet>& p) { }

private:
    Ipv4Address value;
    
};

#endif