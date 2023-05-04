#include "filter.h"
#include "filter-element.h"
#include "ns3/packet.h"

using namespace ns3;
using namespace std;


namespace ns3
{

  std::vector<FilterElement*> elements;
  Filter::Filter(const std::vector<FilterElement*>& elements) : elements(elements) {}

  bool Filter::Match(const Ptr<Packet>& p) const {
    for (const auto& elem : elements) {
      if (!elem->match(p)) {
        return false;
      }
    }
    return true;
  }

}