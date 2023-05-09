#ifndef FILTER_H
#define FILTER_H

#include <vector>
#include "ns3/packet.h"
#include "filter-element.h"

using namespace ns3;
using namespace std;
namespace ns3 {

  class FilterContainer: public Object{
    public:
      FilterContainer() {};
      bool match(const Ptr<Packet> p);

      void addElement(FilterElement* element);

    private:
      vector<FilterElement*> elements;
  };

};

#endif /* FILTER_H */