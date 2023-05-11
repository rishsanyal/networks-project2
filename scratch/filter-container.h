#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/packet.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;

namespace ns3 {

  class FilterContainer{
    public:
      FilterContainer() {};
      bool match(const Ptr<Packet> p);
      void addElement(ns3::FilterElement* element);

    private:
      vector<FilterElement*> elements;
  };


  void FilterContainer::addElement(ns3::FilterElement* element) {
    elements.push_back(element);
  }

  bool FilterContainer::match(const Ptr<Packet> p) {
    for (const auto& elem : elements) {
      if (!elem->match(p)) {
        return false;
      }
    }

    return true;

  }

}

#endif /* FILTER_H */