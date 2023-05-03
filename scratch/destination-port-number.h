#ifndef DESTINATION_PORT_NUMBER_H
#define DESTINATION_PORT_NUMBER_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class DestinationPortNumber: public FilterElement {
public:
    DestinationPortNumber() {}

    bool match(Ptr<Packet> p) { }

private:
    uint32_t value;
};

#endif