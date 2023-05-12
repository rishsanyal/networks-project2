
#ifndef NEW_TRAFFIC_CLASS_H
#define NEW_TRAFFIC_CLASS_H

#include <cstdint>
#include <vector>
#include <queue>

#include "ns3/packet.h"
#include "filter-container.h"

using namespace ns3;
using namespace std;

namespace ns3
{
class NewTrafficClass{
    public:
        NewTrafficClass(uint32_t maxPackets = 10, uint32_t maxBytes = 10, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false);
        // NewTrafficClass();
        // ~NewTrafficClass();

        bool Enqueue(Ptr<Packet> p);
        Ptr<Packet> Dequeue();
        int PeekSize();


        bool Match(ns3::Ptr<ns3::Packet> p) const;
        vector<FilterContainer*> filters;
        uint32_t GetPriorityLevel() const;
        bool AddFilter(FilterContainer* f);
        bool isEmpty() const;

        double GetWeight();

    private:
        uint32_t bytes;
        uint32_t packets;
        uint32_t maxPackets;
        uint32_t maxBytes;
        double weight;
        uint32_t priorityLevel;
        bool isDefault;
        queue<Ptr<Packet>> m_queue;

        vector<int> weightVector;
};

NewTrafficClass::NewTrafficClass(uint32_t maxPackets, uint32_t maxBytes, double weight, uint32_t priorityLevel, bool isDefault){
    bytes = 0;
    packets = 0;
    this->maxPackets = maxPackets;
    this->maxBytes = maxBytes;
    this->weight = weight;
    this->priorityLevel = priorityLevel;
    this->isDefault = isDefault;
}

double NewTrafficClass::GetWeight(){
    return weight;
}

int NewTrafficClass::PeekSize(){
    Ptr<Packet> p = m_queue.front();
    return p->GetSize();
}

bool NewTrafficClass::Enqueue(Ptr<Packet> p){

    if(packets >= maxPackets || bytes >= maxBytes){
        return false;
    }

    m_queue.push(p);
    this->packets++;
    this->bytes += p->GetSize();
    return true;
}

Ptr<Packet> NewTrafficClass::Dequeue(){
    if(m_queue.empty()){
        return NULL;
    }
    Ptr<Packet> p = m_queue.front();
    m_queue.pop();
    this->packets--;
    this->bytes -= p->GetSize();
    return p;
}

bool NewTrafficClass::Match(Ptr<Packet> p) const{
    for (auto f : filters){
        if (f->match(p)){
            return true;
        }
    }
    return false;
}

uint32_t NewTrafficClass::GetPriorityLevel() const{
    return priorityLevel;
}

bool NewTrafficClass::AddFilter(FilterContainer* f){
    filters.push_back(f);
    return true;
}

bool NewTrafficClass::isEmpty() const{
    return m_queue.empty();
}

}
#endif /*NEW_TRAFFIC_CLASS_H*/
