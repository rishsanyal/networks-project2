#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/packet.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

class Filter {
public:
  Filter(const std::vector<FilterElement*>& elements);
  std::vector<FilterElement*> elements;
  bool Match(const ns3::Ptr<ns3::Packet>& p) const;
};

#endif /* FILTER_H */