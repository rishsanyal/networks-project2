#ifndef DESTINTATION_MASK_H
#define DESTINTATION_MASK_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std; 

class DestinationMask: public FilterElement {
public:
    DestinationMask(){ }

    bool match(Ptr<Packet> p) { }

private:
    Ipv4Mask value;
};

#endif