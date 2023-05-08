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

        virtual bool Enqueue(Ptr<Packet> p) = 0;
        virtual Ptr<Packet> Dequeue() = 0;

        Ptr<Packet> DoDequeue(int priority){
            // TODO: Check which queue to dequeue from.
            // Unsure what the queue should be.
            for(int i = 0; i < this->q_class.size(); i++){
                if(this->q_class[i]->GetPriority() == priority){
                    return q_class[i]->Dequeue();
                }
            }
        };

        bool DoEnqueue(Ptr<Packet> p){
            // TODO: Check which queue to dequeue from.
            // Unsure what the queue should be.
            int index = this->Classify(p);
            return q_class[index]->Enqueue(p);
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
        bool QisEmpty(int priority){
            for(int i = 0; i < this->q_class.size(); i++){
                if(this->q_class[i]->GetPriority() == priority){
                    if(this->q_class[i]->IsEmpty()){
                        return true;
                    } else {
                        return false;
                    }
                }
            }
            return NULL;
        }

        virtual Ptr<Packet> Schedule();

        // Ptr<Packet> Schedule(){
        //     // TODO: Understand what the schdule method is used for.
            
        //     return this->DoDequeue();
        // }

        uint32_t Classify(Ptr<Packet> p){
            // TODO: Understand what the schdule method is used for.
            for(int i = 0; i < this->q_class.size(); i++){
                if(this->q_class[i]->Match(p)){
                    return i;
                }
            }

            return 0;
        }

        bool Enqueue(Ptr<Packet> p){
            return this->DoEnqueue(p);
        }
        bool Dequeue(int priority){
            return this->DoDequeue(priority);
        }

};


