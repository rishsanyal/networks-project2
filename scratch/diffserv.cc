#include <vector>

#include "ns3/queue.h"
// #include "src/network/utils/queue.h"
#include "ns3/log.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/object-base.h"
#include "traffic-class.h"
#include "queue-mode.cc"

NS_LOG_COMPONENT_DEFINE("DiffServ");

using namespace ns3;
using namespace std;

namespace ns3
{

class DiffServ: public Queue<Packet> {
    private:
        QueueMode m_mode;
        vector<TrafficClass*> q_class;

        // bool Enqueue(Ptr<Packet> p) = 0;
        // virtual Ptr<Packet> Dequeue() = 0;

        virtual Ptr<Packet> DoDequeue(int priority);

        virtual bool DoEnqueue(Ptr<Packet> p);

        Ptr<Packet> DoRemove(){
            // TODO: Check what this means

            return q_class[0]->Dequeue();
        };

        Ptr<const Packet> DoPeek() const{
            // TODO: Check which queue to dequeue from.

            return q_class.front()->Dequeue();
        };

    public:
        DiffServ(uint32_t noOfQueues, vector<int> priorityLevels, QueueMode mode = PacketMode){
            this->m_mode = mode;
            int* minPriority = std::min_element(q_class.begin(), q_class.end());
            for(int i = 0; i < noOfQueues; i++){
                if(priorityLevels[i] == minPriority) {
                    this->q_class.push_back(new TrafficClass(100, 50000, 0.0, priorityLevels[i], true));
                }
                this->q_class.push_back(new TrafficClass(100, 50000, 0.0, priorityLevels[i], false));
            }
        }

        void SetMode(QueueMode mode){
            this->m_mode = mode;
        }

        QueueMode GetMode(){
            return this->m_mode;
        }

        //getter function for q_class
        virtual bool QisEmpty(int priority);

        virtual Ptr<Packet> Schedule();

        // Ptr<Packet> Schedule(){
        //     // TODO: Understand what the schdule method is used for.

        //     return this->DoDequeue();
        // }

        virtual uint32_t Classify(Ptr<Packet> p);

        bool Enqueue(Ptr<Packet> p) override{
            return this->DoEnqueue(p);
        }
        bool Dequeue(int priority){
            return this->DoDequeue(priority);
        }

        vector<TrafficClass*> GetQClass() const {
            return this->q_class;
        }

};
} // namespace name

