#include "filter-element.h"

class FilterElement {
public:
  FilterElement() {}
  virtual bool match(const ns3::Ptr<ns3::Packet>& p) {
    cout << "FilterElement::match()" << endl;
    return false;
  }
};