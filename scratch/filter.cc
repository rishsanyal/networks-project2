#include "Filter.h"
#include "filter-element.h"
#include "ns3/packet.h"

using namespace ns3;
using namespace std;

class Filter {
public:
  Filter(const vector<FilterElement*>& elements) : elements(elements) {}
  bool Match(const Ptr<Packet>& p) const {
    for (const auto& elem : elements) {
      if (!elem->match(p)) {
        return false;
      }
    }
    return true;
  }
  std::vector<FilterElement*> elements;
};