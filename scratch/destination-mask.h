#ifndef DESTINTATION_MASK_H
#define DESTINTATION_MASK_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class DestinationIPMask: public FilterElement {
public:
    DestinationIPMask(){ }

    bool match(Ptr<Packet> p) { }
    void setValue(Ipv4Address address, Ipv4Mask value){}

private:
    Ipv4Mask value;
};

#endif