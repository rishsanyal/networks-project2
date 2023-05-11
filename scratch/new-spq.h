#ifndef NEW_SPQ_H
#define NEW_SPQ_H

#include "ns3/packet.h"

// #include <fstream>
#include "new-diffserv.h"
// #include "queue-mode.cc"
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
    // ~NewPriQueue() override;

    bool Enqueue(Ptr<ns3::Packet> p) override;
    Ptr<ns3::Packet> Dequeue() override;
    Ptr<ns3::Packet> Remove() override;
    Ptr<const ns3::Packet> Peek() const override;

    Ptr<ns3::Packet> Schedule() override;
    uint32_t Classify(Ptr<ns3::Packet> p) override;

    void test();

    using NewDiffServ::AddTrafficClass;
    using NewDiffServ::EnqueueAtIndex;
    using NewDiffServ::checkForPacketInAllTrafficClasses;
    using NewDiffServ::GetTrafficClasses;


private:
    Ptr<ns3::Packet> DoDequeue(int priorityLevel);

    bool DoEnqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> DoRemove();
    Ptr<const ns3::Packet> DoPeek() const;

    int noOfQueues;
    vector<int> priorityLevels;
    vector<NewTrafficClass*> q_class;
    QueueMode m_mode;
};

NewPriQueue::NewPriQueue() {
    cout << "NewPriQueue" << endl;
    noOfQueues = 0;
    priorityLevels = vector<int>();
    q_class = vector<NewTrafficClass*>();
    m_mode = QueueMode::Packet; //TODO: Get this from the config file?
}

bool NewPriQueue::Enqueue(Ptr<ns3::Packet> p){
    cout << "NewPriQueue::Enqueue" << endl;

    int vectorIndex = Classify(p);

    if (vectorIndex == -1) {
        return false;
    }

    bool enqueueStatus = EnqueueAtIndex(p, vectorIndex);

    // if (enqueueStatus) {
    //     priorityMap.insert(std::pair<int, int>(vectorIndex, priorityLevels[vectorIndex]));
    // }

    return enqueueStatus;
}

Ptr<ns3::Packet> NewPriQueue::Dequeue(){
    cout << "NewPriQueue::Dequeue" << endl;

    return Schedule();
}


Ptr<ns3::Packet> NewPriQueue::Remove(){
    cout << "NewPriQueue::Remove" << endl;
    return DoRemove();
}

Ptr<const ns3::Packet> NewPriQueue::Peek() const{
    cout << "NewPriQueue::Peek" << endl;
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

Ptr<ns3::Packet> NewPriQueue::DoDequeue(int priorityLevel){
    cout << "NewPriQueue::DoDequeue " << endl;
    cout << "priorityLevel: " << priorityLevel << endl;
    return nullptr;
}

bool NewPriQueue::DoEnqueue(Ptr<ns3::Packet> p){
    // cout << "NewPriQueue::DoEnqueue" << endl;
    return true;
}

Ptr<ns3::Packet> NewPriQueue::DoRemove(){
    cout << "NewPriQueue::DoRemove" << endl;
    return nullptr;
}

Ptr<const ns3::Packet> NewPriQueue::DoPeek() const{
    cout << "NewPriQueue::DoPeek" << endl;
    // return nullptr;

    Ptr<const ns3::Packet> item = NULL;
    return item;
}

void NewPriQueue::test(){
    cout << "NewPriQueue::test" << endl;
}

} // namespace ns3

#endif // NEW_SPQ_H