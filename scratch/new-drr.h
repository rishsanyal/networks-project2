#ifndef NEW_DRR_H
#define NEW_DRR_H

#include "ns3/packet.h"

#include "new-diffserv.h"
#include <stdint.h>

using namespace std;

namespace ns3
{

class NewDRRQueue : public NewDiffServ
{
public:
    NewDRRQueue();
    // ~NewDRRQueue() override;

    bool Enqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> Dequeue();
    Ptr<ns3::Packet> Remove();
    Ptr<const ns3::Packet> Peek() const;

    Ptr<ns3::Packet> Schedule();
    uint32_t Classify(Ptr<ns3::Packet> p);

    void AddTrafficClass(NewTrafficClass* trafficClass);

    // using NewDiffServ::AddTrafficClass;
    using NewDiffServ::EnqueueAtIndex;
    using NewDiffServ::checkForPacketInAllTrafficClasses;
    using NewDiffServ::GetTrafficClasses;


private:
    Ptr<ns3::Packet> DoDequeue();

    bool DoEnqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> DoRemove();
    Ptr<const ns3::Packet> DoPeek() const;

    int noOfQueues;
    vector<int> priorityLevels;
    // vector<NewTrafficClass*> q_class;
    vector<int> weightTracker;
    QueueMode m_mode;

    bool checkEmptyTrafficClasses();

    int currentQueueIndex = 0;
};

NewDRRQueue::NewDRRQueue() {
    noOfQueues = 0;
    weightTracker = vector<int>();
    m_mode = QueueMode::ByteMode;
}

/*
    Return true if all the traffic classes are empty
*/
bool
NewDRRQueue::checkEmptyTrafficClasses(){
    vector<NewTrafficClass*> trafficClasses = GetTrafficClasses();

    for (int i = 0; i < trafficClasses.size(); i++) {
        if (!trafficClasses[i]->isEmpty()) {
            return false;
        }
    }

    return true;
}

bool NewDRRQueue::Enqueue(Ptr<ns3::Packet> p){
    int vectorIndex = Classify(p);

    if (vectorIndex == -1) {
        return false;
    }

    bool enqueueStatus = EnqueueAtIndex(p, vectorIndex);

    return enqueueStatus;
}

Ptr<ns3::Packet> NewDRRQueue::Dequeue(){
    Ptr<ns3::Packet> dequedPacket = DoDequeue();
    return dequedPacket;
}

Ptr<ns3::Packet> NewDRRQueue::Remove(){
    return DoRemove();
}

Ptr<const ns3::Packet> NewDRRQueue::Peek() const{
    return DoPeek();
}

Ptr<ns3::Packet> NewDRRQueue::Schedule(){

    if (checkEmptyTrafficClasses()) {
        return nullptr;
    }

    int currPacketSize;
    vector<NewTrafficClass*> trafficClasses = GetTrafficClasses();


    while (true) {
        if (trafficClasses[currentQueueIndex]->isEmpty()) {
            currentQueueIndex = (currentQueueIndex + 1) % noOfQueues;
            continue;
        }

        else{
                currPacketSize = trafficClasses[currentQueueIndex]->PeekSize();
                if (currPacketSize <= weightTracker[currentQueueIndex]) {
                    weightTracker[currentQueueIndex] -= currPacketSize;
                    break;
                }

                else if (currPacketSize > weightTracker[currentQueueIndex]) {
                    weightTracker[currentQueueIndex] += trafficClasses[currentQueueIndex]->GetWeight();
                    currentQueueIndex = (currentQueueIndex + 1) % noOfQueues;
                }
        }
    }

    return DequeueFromIndex(currentQueueIndex);
}

uint32_t NewDRRQueue::Classify(Ptr<ns3::Packet> p){
    int trafficClassIndex = checkForPacketInAllTrafficClasses(p);
    return trafficClassIndex;
}

Ptr<ns3::Packet> NewDRRQueue::DoDequeue(){
    Ptr<ns3::Packet> dequedPacket = Schedule();
    return dequedPacket;
}

bool NewDRRQueue::DoEnqueue(Ptr<ns3::Packet> p){
    return Enqueue(p);
}

Ptr<ns3::Packet> NewDRRQueue::DoRemove(){
    return Dequeue();
}

Ptr<const ns3::Packet> NewDRRQueue::DoPeek() const{
    vector<NewTrafficClass*> trafficClasses = GetTrafficClasses();
    Ptr<const ns3::Packet> item = trafficClasses[currentQueueIndex]->Peek();
    return item;
}

void NewDRRQueue::AddTrafficClass(NewTrafficClass* trafficClass){
    noOfQueues++;
    weightTracker.push_back(trafficClass->GetWeight());

    NewDiffServ::AddTrafficClass(trafficClass);
} // namespace ns3
}
#endif // NEW_DRR_H