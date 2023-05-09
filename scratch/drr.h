#ifndef DRR_H
#define DRR_H

#include <fstream>
#include <vector>
#include <memory>
#include "diffserv.h"
#include "traffic-class.h"

namespace ns3 {

class DRR {
    private:
        int noOfQueues;
        std::vector<int> quantumValues;
        std::unique_ptr<DiffServ> diffServ;
        std::vector<std::unique_ptr<TrafficClass>> q_class;
        std::vector<int> deficitCounter;

public:
    DRR(int noOfQueues, std::vector<int> quantumValues);
    bool Enqueue(const Ptr<Packet>& p);
    Ptr<Packet> Dequeue();
    uint32_t Classify(Ptr<Packet> p);

private:
    bool QisEmpty(int i) const;
};

} // namespace ns3

#endif // DRR_H





