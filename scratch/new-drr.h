#ifndef NEW_DRR_H
#define NEW_DRR_H

#include "ns3/packet.h"

// #include <fstream>
#include "new-diffserv.h"
// #include "queue-mode.cc"
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
    void test();

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
    vector<NewTrafficClass*> q_class;
    vector<int> weightTracker;
    QueueMode m_mode;

    int currentQueueIndex = 0;
};

NewDRRQueue::NewDRRQueue() {
    cout << "NewDRRQueue" << endl;
    noOfQueues = 0;
    weightTracker = vector<int>();
    q_class = vector<NewTrafficClass*>();
    m_mode = QueueMode::ByteMode; //TODO: Get this from the config file?
}

bool NewDRRQueue::Enqueue(Ptr<ns3::Packet> p){
    cout << "NewDRRQueue::Enqueue" << endl;

    int vectorIndex = Classify(p);

    if (vectorIndex == -1) {
        return false;
    }

    bool enqueueStatus = EnqueueAtIndex(p, vectorIndex);
    
    return enqueueStatus;
}

Ptr<ns3::Packet> NewDRRQueue::Dequeue(){
    std::cout << "NewDRRQueue::Dequeue" << std::endl;

    return Schedule();
}

Ptr<ns3::Packet> NewDRRQueue::Remove(){
    cout << "NewDRRQueue::Remove" << endl;
    return DoRemove();
}

Ptr<const ns3::Packet> NewDRRQueue::Peek() const{
    cout << "NewDRRQueue::Peek" << endl;
    return DoPeek();
}

Ptr<ns3::Packet> NewDRRQueue::Schedule(){
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
    cout << "NewDRRQueue::DoDequeue " << endl;

    return nullptr;
}

bool NewDRRQueue::DoEnqueue(Ptr<ns3::Packet> p){
    // cout << "NewDRRQueue::DoEnqueue" << endl;
    return true;
}

Ptr<ns3::Packet> NewDRRQueue::DoRemove(){
    cout << "NewDRRQueue::DoRemove" << endl;
    return nullptr;
}

Ptr<const ns3::Packet> NewDRRQueue::DoPeek() const{
    cout << "NewDRRQueue::DoPeek" << endl;
    return nullptr;
}

void NewDRRQueue::test(){
    cout << "NewDRRQueue::test" << endl;
}

void NewDRRQueue::AddTrafficClass(NewTrafficClass* trafficClass){
    // cout << "NewDRRQueue::AddTrafficClass" << endl;
    noOfQueues++;
    weightTracker.push_back(trafficClass->GetWeight());

    NewDiffServ::AddTrafficClass(trafficClass);
} // namespace ns3
}
#endif // NEW_DRR_H