#include <vector>

#include "ns3/queue.h"
// #include "src/network/utils/queue.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/object-base.h"
#include "traffic-class.h"
#include "queue-mode.cc"

NS_LOG_COMPONENT_DEFINE("DiffServ");

using namespace ns3;
using namespace std;

class DiffServ: public Queue<Packet> {
    private:
        QueueMode m_mode;
        vector<TrafficClass*> q_class;

        Ptr<Packet> DoDequeue(){
            // TODO: Check which queue to dequeue from.
            // Unsure what the queue should be.


            return q_class[0]->Dequeue();
        };

        bool DoEnqueue(Ptr<Packet> p){
            // TODO: Check which queue to dequeue from.
            // Unsure what the queue should be.

            return q_class[0]->Enqueue(p);
        };

        Ptr<Packet> DoRemove(){
            // TODO: Check what this means

            return q_class[0]->Dequeue();
        };

        Ptr<const Packet> DoPeek() const{
            // TODO: Check which queue to dequeue from.

            return q_class.front()->Dequeue();
        };

    public:
        DiffServ(QueueMode mode = PacketMode){
            this->m_mode = mode;
            this->q_class = vector<TrafficClass*>();
        }

        void SetMode(QueueMode mode){
            this->m_mode = mode;
        }

        QueueMode GetMode(){
            return this->m_mode;
        }

        Ptr<Packet> Schedule(){
            // TODO: Understand what the schdule method is used for.
            return this->DoDequeue();
        }

        uint32_t Classify(Ptr<Packet> p){
            // TODO: Understand what the schdule method is used for.
            return 0;
        }

};