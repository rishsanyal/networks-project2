#ifndef NEW_DIFFSERV_H
#define NEW_DIFFSERV_H


#include <vector>

#include "ns3/queue.h"
#include "ns3/packet.h"
// #include "src/network/utils/queue.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/object-base.h"
#include "new-traffic-class.h"
// #include "queue-mode.cc"


#include <typeinfo>

// NS_LOG_COMPONENT_DEFINE("NewDiffServ");

using namespace std;

namespace ns3
{

class NewDiffServ: public Queue<ns3::Packet> {
public:


    enum QueueMode {
        ByteMode, Packet
    };

    NewDiffServ();
    // ~NewDiffServ() override;

    bool Enqueue(Ptr<Packet> item) override;
    Ptr<Packet> Dequeue() override;
    Ptr<Packet> Remove() override;
    Ptr<const Packet> Peek() const override;

    virtual Ptr<ns3::Packet> Schedule();
    virtual uint32_t Classify(Ptr<ns3::Packet> p);

    void AddTrafficClass(NewTrafficClass* trafficClass);
    int checkForPacketInAllTrafficClasses(Ptr<ns3::Packet> p);
    void addPacketToTrafficClass(Ptr<ns3::Packet> p, int vectorIndex);


    bool EnqueueAtIndex(Ptr<ns3::Packet> p, int vectorIndex);
    Ptr<Packet> DequeueFromIndex(int vectorIndex);

    vector<NewTrafficClass*> GetTrafficClasses();


    void SetMode(NewDiffServ::QueueMode mode);
    NewDiffServ::QueueMode GetMode();


    // For key we know the index of the vector
    // std::map<uint32_t, int> priorityMap;
    // priority_queue <tuple<int,int>> pq;

private:
    Ptr<ns3::Packet> DoDequeue();
    bool DoEnqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> DoRemove();
    Ptr<const ns3::Packet> DoPeek() const;

    NewDiffServ::QueueMode m_mode;
    vector<NewTrafficClass*> q_class;
};


NewDiffServ::NewDiffServ() {
    // NS_LOG_DEBUG("NewDiffServ");
    m_mode = NewDiffServ::QueueMode::Packet;
    q_class = vector<NewTrafficClass*>();
}

bool NewDiffServ::Enqueue(Ptr<Packet> item) {

    // Classify returns the index at which we need to add the packet
    int vectorIndex = Classify(item);

    if (vectorIndex == -1) {
        return false;
    }

    return EnqueueAtIndex(item, vectorIndex);
}

Ptr<Packet> NewDiffServ::Dequeue() {
    // NS_LOG_FUNCTION(this);
    return DoDequeue();
}

Ptr<Packet> NewDiffServ::Remove() {
    // NS_LOG_FUNCTION(this);
    return DoRemove();
}

Ptr<const Packet> NewDiffServ::Peek() const {
    // NS_LOG_FUNCTION(this);
    return DoPeek();
}

void NewDiffServ::SetMode(QueueMode mode) {
    // NS_LOG_FUNCTION(this << mode);
    m_mode = mode;
}

NewDiffServ::QueueMode NewDiffServ::GetMode() {
    // NS_LOG_FUNCTION(this);
    return m_mode;
}

vector<NewTrafficClass*> NewDiffServ::GetTrafficClasses() {
    return q_class;
}

Ptr<ns3::Packet> NewDiffServ::Schedule() {
    // NS_LOG_FUNCTION(this);
    return DoDequeue();
}

uint32_t NewDiffServ::Classify(Ptr<ns3::Packet> p) {
    // NS_LOG_FUNCTION(this << p);
    int trafficClassIndex = checkForPacketInAllTrafficClasses(p);
    return trafficClassIndex;
}

void NewDiffServ::AddTrafficClass(NewTrafficClass* trafficClass) {
    // NS_LOG_FUNCTION(this << trafficClass);

    // int currentSize = q_class.size();
    // add currentSize to the map with the key as the priority

    // priorityMap.insert(std::pair<uint32_t, int>(trafficClass->GetPriorityLevel(), currentSize));

    // Get the weight of the TrafficClass and add that to our weightTracker

    q_class.push_back(trafficClass);
}

Ptr<ns3::Packet> NewDiffServ::DoDequeue() {
    // NS_LOG_FUNCTION(this);
    cout << "DoDequeue" << endl;
    return nullptr;
}

bool NewDiffServ::DoEnqueue(Ptr<ns3::Packet> p) {
    // NS_LOG_FUNCTION(this << p);
    cout << "DoEnqueue" << endl;
    return true;
}


bool NewDiffServ::EnqueueAtIndex(Ptr<ns3::Packet> p, int vectorIndex){
    // NS_LOG_FUNCTION(this);
    cout << "Enqueue at index: ";
    cout << vectorIndex << endl;

    return q_class[vectorIndex]->Enqueue(p);;
}


int NewDiffServ::checkForPacketInAllTrafficClasses(Ptr<ns3::Packet> p) {
    for (int i = 0; i < q_class.size(); i++) {
        if (q_class[i]->Match(p)) {
            return i;
        }
    }

    return -1;
}

Ptr<ns3::Packet> NewDiffServ::DequeueFromIndex(int vectorIndex) {
    cout << "Dequeue from index: " << vectorIndex << endl;
    return q_class[vectorIndex]->Dequeue();
}


Ptr<ns3::Packet> NewDiffServ::DoRemove() {
    // NS_LOG_FUNCTION(this);
    cout << "DoRemove" << endl;
    return nullptr;
}

Ptr<const ns3::Packet> NewDiffServ::DoPeek() const {
    // NS_LOG_FUNCTION(this);
    cout << "DoPeek" << endl;
    return nullptr;
}



} // namespace name


#endif /*NEW_DIFFSERV_H*/