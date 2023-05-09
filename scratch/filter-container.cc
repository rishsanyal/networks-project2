#include "filter-container.h"
#include "filter-element.h"

#include "ns3/packet.h"

using namespace ns3;
using namespace std;

namespace ns3 {

  // FilterContainer::FilterContainer() {};

  void FilterContainer::addElement(FilterElement* element) {
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
    // class FilterContainer {
    //   private:
    //     vector<FilterElement*> elements;

    //   public:
    //       FilterContainer(){};

    //       void addElement(FilterElement* element) {
    //         elements.push_back(element);
    //       }

    //     bool match(const Ptr<Packet>& p) const {
    //       for (const auto& elem : elements) {
    //         if (!elem->match(p)) {
    //           return false;
    //         }
    //       }
    //       return true;
    //     }

    // };
};

// class FilterContainer {
//   private:
//     std::vector<FilterElement*> elements;

//   public:
//     // Filter(const vector<FilterElement*>& elements) : elements(elements) {}

//     FilterContainer() {}

//     void addElement(FilterElement* element) {
//       elements.push_back(element);
//     }

//     bool match(const Ptr<Packet>& p) const {
//       for (const auto& elem : elements) {
//         if (!elem->match(p)) {
//           return false;
//         }
//       }
//       return true;
//     }

// };