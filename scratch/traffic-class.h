#ifndef TRAFFIC_CLASS_H
#define TRAFFIC_CLASS_H

#include <cstdint>
#include <vector>
#include <queue>

#include "ns3/packet.h"
#include "filter.h"

using namespace ns3;
using namespace std;

class TrafficClass{
public:
    TrafficClass(uint32_t maxPackets, uint32_t maxBytes, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false);
    bool Enqueue(const Ptr<Packet> p);
    Ptr<Packet> Dequeue();
    bool Match(Ptr<Packet> p) const;
    vector<Filter*> filters;

private:
    uint32_t bytes;
    uint32_t packets;
    uint32_t maxPackets;
    uint32_t maxBytes;
    double weight;
    uint32_t priorityLevel;
    bool isDefault;
    queue<Ptr<Packet>> m_queue;
};

#endif
