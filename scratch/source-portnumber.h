#ifndef SOURCE_PORTNUMBER_H
#define SOURCE_PORTNUMBER_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class SourcePortNumber: public FilterElement {
public:
    SourcePortNumber() {}

    bool match(Ptr<Packet> p) { }

private:
    uint32_t value;
};

#endif