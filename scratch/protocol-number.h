
#ifndef DESTINATION_IP_ADDRESS_H
#define DESTINATION_IP_ADDRESS_H

#include "ns3/core-module.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class ProtocolNumber : public FilterElement {
    public:
        ProtocolNumber();
        void setValue(uint32_t value);
        bool match(Ptr<Packet> p) override;

    private:
            uint32_t value;

};

#endif