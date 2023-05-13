#ifndef NEW_SPQ_H
#define NEW_SPQ_H

#include "ns3/packet.h"

#include "new-diffserv.h"
#include <stdint.h>

using namespace std;

namespace ns3
{

class NewPriQueue : public NewDiffServ
{

enum QueueMode {
    ByteMode, Packet
};


public:
    NewPriQueue();

    bool Enqueue(Ptr<ns3::Packet> p) override;
    Ptr<ns3::Packet> Dequeue() override;
    Ptr<ns3::Packet> Remove() override;
    Ptr<const ns3::Packet> Peek() const override;

    Ptr<ns3::Packet> Schedule() override;
    uint32_t Classify(Ptr<ns3::Packet> p) override;

    using NewDiffServ::AddTrafficClass;
    using NewDiffServ::EnqueueAtIndex;
    using NewDiffServ::checkForPacketInAllTrafficClasses;
    using NewDiffServ::GetTrafficClasses;


private:
    bool DoEnqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> DoRemove();
    Ptr<ns3::Packet> DoDequeue();
    Ptr<const ns3::Packet> DoPeek() const;

    int noOfQueues;
    vector<int> priorityLevels;
    vector<NewTrafficClass*> q_class;
    QueueMode m_mode;
};

NewPriQueue::NewPriQueue() {
    noOfQueues = 0;
    priorityLevels = vector<int>();
    q_class = vector<NewTrafficClass*>();
    m_mode = QueueMode::Packet; //TODO: Get this from the config file?
}

bool NewPriQueue::Enqueue(Ptr<ns3::Packet> p){
    int vectorIndex = Classify(p);

    if (vectorIndex == -1) {
        return false;
    }

    bool enqueueStatus = EnqueueAtIndex(p, vectorIndex);

    return enqueueStatus;
}

Ptr<ns3::Packet> NewPriQueue::Dequeue(){
    return DoDequeue();
}

Ptr<ns3::Packet> NewPriQueue::DoDequeue(){
    return Schedule();
}


Ptr<ns3::Packet> NewPriQueue::Remove(){
    return DoRemove();
}

Ptr<const ns3::Packet> NewPriQueue::Peek() const{
    return DoPeek();
}


Ptr<ns3::Packet> NewPriQueue::Schedule(){
    // Gets the index of the traffic class that has the highest priority
    // and has a packet to send

    vector<NewTrafficClass*> trafficClasses = GetTrafficClasses();

    uint32_t curr_highest_priority_index_with_packets = UINT32_MAX;
    size_t index = -1;


    for (size_t i = 0; i < trafficClasses.size(); i++) {
        if (!trafficClasses[i]->isEmpty()) {
            if (trafficClasses[i]->GetPriorityLevel() < curr_highest_priority_index_with_packets) {
                curr_highest_priority_index_with_packets = trafficClasses[i]->GetPriorityLevel();
                index = i;
            }
        }
    }

    if (index == -1) {
        return nullptr;
    }

    return DequeueFromIndex(index);
}

uint32_t NewPriQueue::Classify(Ptr<ns3::Packet> p){
    int trafficClassIndex = checkForPacketInAllTrafficClasses(p);
    return trafficClassIndex;
}

bool NewPriQueue::DoEnqueue(Ptr<ns3::Packet> p){
    return Enqueue(p);
}

Ptr<ns3::Packet> NewPriQueue::DoRemove(){
    return Dequeue();
}

Ptr<const ns3::Packet> NewPriQueue::DoPeek() const{
    cout << "NewPriQueue::DoPeek" << endl;

    vector<NewTrafficClass*> trafficClasses = GetTrafficClasses();

    uint32_t curr_highest_priority_index_with_packets = UINT32_MAX;
    size_t index = -1;


    for (size_t i = 0; i < trafficClasses.size(); i++) {
        if (!trafficClasses[i]->isEmpty()) {
            if (trafficClasses[i]->GetPriorityLevel() < curr_highest_priority_index_with_packets) {
                curr_highest_priority_index_with_packets = trafficClasses[i]->GetPriorityLevel();
                index = i;
            }
        }
    }

    if (index == -1) {
        return nullptr;
    }

    Ptr<const ns3::Packet> item = trafficClasses[index]->Peek();
    return item;
}

} // namespace ns3

#endif // NEW_SPQ_H