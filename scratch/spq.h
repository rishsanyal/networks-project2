#ifndef SPQ_H
#define SPQ_H

#include <fstream>
#include "diffserv.h"
#include "traffic-class.h"
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

using namespace ns3;

namespace ns3 {
class SPQ: public DiffServ {
public:
  SPQ();

  bool Enqueue(Ptr<Packet> p) override;

  Ptr<Packet> Dequeue() override;

  Ptr<Packet> Schedule() override;

  Ptr<Packet> Remove() override;
  Ptr<const Packet> Peek() const override;

// private:
//   std::vector<TrafficClass> queues_;
//   std::unordered_map<uint32_t, uint32_t> queue_map_; // maps priority level to queue index
//   uint32_t highest_priority_ = std::numeric_limits<uint32_t>::max();
// };
};
} // namespace ns3
#endif // SPQ_H
