#ifndef SPQ_H
#define SPQ_H

#include <fstream>
#include<diffserv.h>
#include "traffic-class.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class SPQ: public DiffServ {
public:
  SPQ();

  bool Enqueue(const Ptr<Packet>& p);

  Ptr<Packet> Dequeue();

  Ptr<Packet> Schedule() override;

// private:
//   std::vector<TrafficClass> queues_;
//   std::unordered_map<uint32_t, uint32_t> queue_map_; // maps priority level to queue index
//   uint32_t highest_priority_ = std::numeric_limits<uint32_t>::max();
// };
};
#endif // SPQ_H
