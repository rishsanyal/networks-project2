#ifndef SPQ_H
#define SPQ_H

#include <vector>
#include <ns3/packet.h>
#include <ns3/diffserv.h>
#include "traffic-class.h"

namespace ns3 {

class SPQ : public DiffServ {
public:
    SPQ(int noOfQueues, std::vector<int> quantumValues);
    bool Enqueue(const ns3::Ptr<ns3::Packet>& p) override;
    ns3::Ptr<ns3::Packet> Dequeue() override;
    bool QisEmpty(int priority) const override;
    ns3::Ptr<ns3::Packet> Schedule() override;
    uint32_t Classify(ns3::Ptr<ns3::Packet> p) override;

private:
    int m_noOfQueues;
    std::vector<int> m_quantumValues;
    std::vector<TrafficClass*> m_q_class;
    std::vector<int> m_deficitCounters;
    int m_currentQueue;
};

} // namespace ns3

#endif /* SPQ_H */
