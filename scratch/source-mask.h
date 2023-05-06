#ifndef SOURCE_MASK_H
#define SOURCE_MASK_H

#include "ns3/core-module.h"
#include "ns3/ipv4-address.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class SourceMask : public FilterElement {
    private:
        Ipv4Mask value;
    public:
        SourceMask();
        void setValue(Ipv4Mask value);
        bool match(Ptr<Packet> p) override;
};

#endif