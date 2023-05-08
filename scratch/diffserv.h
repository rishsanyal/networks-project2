#ifndef DIFFSERV_H
#define DIFFSERV_H


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
public:
    DiffServ();
    // virtual ~DiffServ();

    void SetMode(QueueMode mode);
    QueueMode GetMode();
    virtual Ptr<Packet> Schedule();
    uint32_t Classify(Ptr<Packet> p);

private:
    Ptr<Packet> DoDequeue();
    bool DoEnqueue(Ptr<Packet> p);
    Ptr<Packet> DoRemove();
    Ptr<const Packet> DoPeek() const;

    QueueMode m_mode;

    // TODO: Needs to be imported
    vector<TrafficClass*> q_class;
};

#endif