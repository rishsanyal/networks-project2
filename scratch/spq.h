#ifndef SPQ_H
#define SPQ_H

#include <fstream>
#include "diffserv.h"
#include "traffic-class.h"
// #include <nlohmann/json.hpp>

// using json = nlohmann::json;

using namespace ns3;

// namespace ns3 {
class SPQ: public DiffServ {
public:
  SPQ();

  bool Enqueue(Ptr<ns3::Packet> p) override;

  Ptr<ns3::Packet> Dequeue() override;

  Ptr<ns3::Packet> Schedule() override;

  Ptr<ns3::Packet> Remove() override;
  Ptr<const ns3::Packet> Peek() const override;

  private:
    int noOfQueues;
    vector<int> priorityLevels;
    DiffServ* diffServ;
    vector<TrafficClass*> q_class;

};
// } // namespace ns3
#endif // SPQ_H
