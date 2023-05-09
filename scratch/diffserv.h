#ifndef DIFFSERV_H
#define DIFFSERV_H


#include <vector>

#include "ns3/queue.h"
#include "ns3/packet.h"
// #include "src/network/utils/queue.h"
#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/object-base.h"
#include "traffic-class.h"
#include "queue-mode.cc"

NS_LOG_COMPONENT_DEFINE("DiffServ");

using namespace ns3;
using namespace std;

namespace ns3
{
    class DiffServ: public Queue<ns3::Packet> {
    public:
        DiffServ();
        // virtual ~DiffServ();
        bool Enqueue(Ptr<Packet> p) override;
        void SetMode(QueueMode mode);
        QueueMode GetMode();
        virtual Ptr<ns3::Packet> Schedule();
        uint32_t Classify(Ptr<ns3::Packet> p);
        void AddTrafficClass();

        using Queue<ns3::Packet>::Remove;
        using Queue<ns3::Packet>::Peek;

    private:
        Ptr<ns3::Packet> DoDequeue();
        bool DoEnqueue(Ptr<ns3::Packet> p);
        Ptr<ns3::Packet> DoRemove();
        Ptr<const ns3::Packet> DoPeek() const;


        QueueMode m_mode;

        vector<TrafficClass*> q_class;
    };

} // namespace name


#endif