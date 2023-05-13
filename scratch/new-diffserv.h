#ifndef NEW_DIFFSERV_H
#define NEW_DIFFSERV_H


#include <vector>

#include "ns3/queue.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/object-base.h"
#include "new-traffic-class.h"

#include <typeinfo>

using namespace std;

namespace ns3
{

class NewDiffServ: public Queue<Packet> {
public:

    /*
        TODO: Make QueueMode ENUM common for all files.
    */
    enum QueueMode {
        ByteMode, Packet
    };

    NewDiffServ();
    // ~NewDiffServ() override;

    bool Enqueue(Ptr<ns3::Packet> item) override;
    Ptr<ns3::Packet> Dequeue() override;
    Ptr<ns3::Packet> Remove() override;
    Ptr<const ns3::Packet> Peek() const override;

    virtual Ptr<ns3::Packet> Schedule();
    virtual uint32_t Classify(Ptr<ns3::Packet> p);

    void AddTrafficClass(NewTrafficClass* trafficClass);
    int checkForPacketInAllTrafficClasses(Ptr<ns3::Packet> p);
    void addPacketToTrafficClass(Ptr<ns3::Packet> p, int vectorIndex);

    bool EnqueueAtIndex(Ptr<ns3::Packet> p, int vectorIndex);
    Ptr<ns3::Packet> DequeueFromIndex(int vectorIndex);

    vector<NewTrafficClass*> GetTrafficClasses() const;

    void SetMode(NewDiffServ::QueueMode mode);
    NewDiffServ::QueueMode GetMode();

    void test();

private:
    Ptr<ns3::Packet> DoDequeue();
    bool DoEnqueue(Ptr<ns3::Packet> p);
    Ptr<ns3::Packet> DoRemove();
    Ptr<const ns3::Packet> DoPeek() const;

    NewDiffServ::QueueMode m_mode;
    vector<NewTrafficClass*> q_class = vector<NewTrafficClass*>();
};


NewDiffServ::NewDiffServ() {
    m_mode = NewDiffServ::QueueMode::Packet;
}

bool NewDiffServ::Enqueue(Ptr<ns3::Packet> item) {

    // Classify returns the index at which we need to add the packet
    int vectorIndex = Classify(item);

    if (vectorIndex == -1) {
        return false;
    }

    return EnqueueAtIndex(item, vectorIndex);
}

Ptr<ns3::Packet> NewDiffServ::Dequeue() {
    return DoDequeue();
}

Ptr<ns3::Packet> NewDiffServ::Remove() {
    return DoRemove();
}

Ptr<const ns3::Packet> NewDiffServ::Peek() const {
    return DoPeek();
}

void NewDiffServ::SetMode(QueueMode mode) {
    m_mode = mode;
}

NewDiffServ::QueueMode NewDiffServ::GetMode() {
    return m_mode;
}

vector<NewTrafficClass*> NewDiffServ::GetTrafficClasses() const{
    return q_class;
}

Ptr<ns3::Packet> NewDiffServ::Schedule() {
    return DoDequeue();
}

uint32_t NewDiffServ::Classify(Ptr<ns3::Packet> p) {
    int trafficClassIndex = checkForPacketInAllTrafficClasses(p);

    return trafficClassIndex;
}

void NewDiffServ::AddTrafficClass(NewTrafficClass* trafficClass) {
    q_class.push_back(trafficClass);
}

Ptr<ns3::Packet> NewDiffServ::DoDequeue() {
    return nullptr;
}

bool NewDiffServ::DoEnqueue(Ptr<ns3::Packet> p) {
    return Enqueue(p);
}


bool NewDiffServ::EnqueueAtIndex(Ptr<ns3::Packet> p, int vectorIndex){
    return q_class[vectorIndex]->Enqueue(p);;
}


int NewDiffServ::checkForPacketInAllTrafficClasses(Ptr<ns3::Packet> p) {

    for (int i = 0; i < q_class.size(); i++) {
        if (q_class[i]->Match(p) ) {
            return i;
        }
    }

    for (int i = 0; i < q_class.size(); i++) {
        if (q_class[i]->GetIsDefault()) {
            return i;
        }
    }

    return -1;
}

Ptr<ns3::Packet> NewDiffServ::DequeueFromIndex(int vectorIndex) {
    return q_class[vectorIndex]->Dequeue();
}


Ptr<ns3::Packet> NewDiffServ::DoRemove() {
    return Dequeue();
}

Ptr<const ns3::Packet> NewDiffServ::DoPeek() const {
    return nullptr;
}

void NewDiffServ::test(){
    cout << "DiffServ::test" << endl;
}

} // namespace name


#endif /*NEW_DIFFSERV_H*/