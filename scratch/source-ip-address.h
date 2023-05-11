#ifndef SOURCE_IP_ADDRESS_H
#define SOURCE_IP_ADDRESS_H

#include "ns3/core-module.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

namespace ns3 {
class SourceIPAddress : public FilterElement {
    public:
        SourceIPAddress();
        void setValue(Ipv4Address value);
        bool match(Ptr<Packet> p) override;

    private:
            Ipv4Address value;

};

SourceIPAddress::SourceIPAddress() {}

void SourceIPAddress::setValue(ns3::Ipv4Address value){
    this->value = value;
}

bool SourceIPAddress::match(Ptr<Packet> p){
    Ipv4Header header;
    p->PeekHeader(header);

    if(header.GetSource() == value){
        // 1
        return true;
    } else {
        // 0
        return false;
    }
}

}

#endif