#include <cstdint>
#include <vector>
#include <queue>
#include <algorithm>

#include "traffic-class.h"
#include "ns3/packet.h"

// #include "filter-container.cc"

using namespace ns3;
using namespace std;

// class TrafficClass{
    // private:
    //     uint32_t bytes; // Current number of bytes in the queue
    //     uint32_t packets; // Current number of packets in the queue
    //     uint32_t maxPackets; // Maximum number of packets in the queue
    //     uint32_t maxBytes; // Maximum number of bytes in the queue
    //     double weight; // Weight of the queue if it is a WFQ queue
    //     uint32_t priorityLevel; // Priority level of the queue
    //     bool isDefault;

    //     queue<Ptr<Packet>> m_queue;

    // public:
        // vector<FilterContainer*> filters;

        TrafficClass::TrafficClass(uint32_t maxPackets, uint32_t maxBytes, double weight, uint32_t priorityLevel, bool isDefault){
            this->bytes = 0;
            this->packets = 0;
            this->maxPackets = maxPackets;
            this->maxBytes = maxBytes;
            this->weight = weight;
            this->priorityLevel = priorityLevel;
            this->isDefault = isDefault;
        }

        bool TrafficClass::Enqueue(const Ptr<Packet> p){
            if (this->packets < this->maxPackets && this->bytes < this->maxBytes){
                this->m_queue.push(p);
                this->packets++;
                this->bytes += p->GetSize();
                return true;
            }
            return false;
        }

        Ptr<Packet> TrafficClass::Dequeue(){
            if (this->m_queue.empty()){
                cout<< "Queue is empty" << endl;
                return NULL;
            }
            Ptr<Packet> p = this->m_queue.front();
            this->m_queue.pop();
            this->packets--;
            this->bytes -= p->GetSize();
            return p;
        }

        bool TrafficClass::Match(Ptr<Packet> p) const{
            for (FilterContainer* f : this->filters){
                if (f->match(p)){
                    return true;
                }
            }
            return false;
        }

        uint32_t TrafficClass::GetPriorityLevel() const{
            return this->priorityLevel;
        }

        bool TrafficClass::AddFilter(FilterContainer* f){
            this->filters.push_back(f);
            return true;
        }

        bool TrafficClass::isEmpty() const{
            return this->m_queue.empty();
        }
// };













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