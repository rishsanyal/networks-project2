#include <iostream>
#include "ns3/core-module.h"
#include "ns3/queue.h"


using namespace std;
using namespace ns3;

#ifndef FILTER_ELEMENT_H
#define FILTER_ELEMENT_H


// NS_LOG_COMPONENT_DEFINE("FilterElement");

namespace ns3
{
class FilterElement : public Object{
public:
    FilterElement(){};
    virtual bool match(ns3::Ptr<ns3::Packet> p) = 0;
};
}

#endif