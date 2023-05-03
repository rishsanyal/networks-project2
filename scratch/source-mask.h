#ifndef SOURCE_MASK_H
#define SOURCE_MASK_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std; 

class SourceMask: public FilterElement {
public:
    SourceMask(){ }

    bool match(Ptr<Packet> p) {}

private:
    Ipv4Mask value;
};

#endif