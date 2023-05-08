#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>

#include "ns3/packet.h"
// #include "filter-container.cc"

using namespace ns3;
using namespace std;

class TrafficClass{
    private:
        uint32_t bytes;
        uint32_t packets;
        uint32_t maxPackets;
        uint32_t maxBytes;
        double weight;
        uint32_t priorityLevel;
        bool isDefault;
        queue<Ptr<Packet>> m_queue;

    public:
        vector<FilterContainer*> filters;

        TrafficClass(uint32_t maxPackets, uint32_t maxBytes, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false){
            this->bytes = 0;
            this->packets = 0;
            this->maxPackets = maxPackets;
            this->maxBytes = maxBytes;
            this->weight = weight;
            this->priorityLevel = priorityLevel;
            this->isDefault = isDefault;
        }

        bool Enqueue(const Ptr<Packet> p){
            if (this->packets < this->maxPackets && this->bytes < this->maxBytes){
                this->m_queue.push(p);
                this->packets++;
                this->bytes += p->GetSize();
                return true;
            }
            return false;
        }

        Ptr<Packet> Dequeue(){
            if (this->m_queue.empty()){
                return NULL;
            }
            Ptr<Packet> p = this->m_queue.front();
            this->m_queue.pop();
            this->packets--;
            this->bytes -= p->GetSize();
            return p;
        }

        bool Match(Ptr<Packet> p) const{
            for (FilterContainer* f : this->filters){
                if (f->match(p)){
                    return true;
                }
            }
            return false;
        }

        uint32_t GetPriorityLevel() const{
            return this->priorityLevel;
        }

        bool isEmpty() const{
            return this->m_queue.empty();
        }
        
};













// I have a list of TrafficClass items, which has the following details: 
//         uint32_t bytes;
//         uint32_t packets;
//         uint32_t maxPackets;
//         uint32_t maxBytes;
//         double weight;
//         uint32_t priorityLevel;
//         bool isDefault;
//         queue<Ptr<Packet>> m_queue;
// Assume that I have written the logic for enqueue, dequeue and match functions, where match checks for different criterias to see if that packet has the same priority as that queue, and returns a boolean value. 
// I have a class called DiffServ that has a vector of TrafficClass objects. So basically, each TrafficClass object has the details for a queue with a specific priority level. 
// I have a subclass called SPQ that inherits from the base class DiffServ. 