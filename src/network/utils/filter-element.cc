#include "filter-element.h"

using namespace std;
// using namespace ns3;

// class FilterElement {
// // public:
//   FilterElement() {}
//   virtual bool match(const ns3::Ptr<ns3::Packet>& p) {
//     cout << "FilterElement::match()" << endl;
//     return false;
//   }
// };

namespace ns3 {
  // FilterElement::FilterElement(){};

  bool FilterElement::match(Ptr<Packet> p) {
    cout << "FilterElement::match()" << endl;
    return false;
  }
}