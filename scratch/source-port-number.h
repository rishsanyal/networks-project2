#ifndef SOURCE_PORTNUMBER_H
#define SOURCE_PORTNUMBER_H

#include "ns3/core-module.h"
#include "ns3/queue.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class SourcePortNumber : public FilterElement {
    public:
        SourcePortNumber();
        void setValue(uint32_t value);
        bool match(Ptr<Packet> p) override;

    private:
            uint32_t value;

};

#endif