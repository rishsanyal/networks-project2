/* CS621: Lab 3 Modified - Two Queues */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"

using namespace ns3;

// NS_LOG_COMPONENT_DEFINE ("MyModifiedScriptExampleWithTwoQueues");

// Custom queue class with two internal queues
class TwoQueues : public Queue<Packet>
{
public:
    // static TypeId GetTypeId (void);
    TwoQueues ();
    ~TwoQueues() override;
//   virtual ~TwoQueues ();
    static TypeId GetTypeId ();
    // virtual Ptr<Packet> DoDequeue (void) ;
    bool Enqueue(Ptr<Packet> item) override;
    Ptr<Packet> Remove() override;
    Ptr<Packet> Dequeue() override;
    Ptr<const Packet> Peek() const override;

private:
//   virtual bool DoEnqueue (Ptr<Packet> p);
//   virtual Ptr<Packet> DoDequeue (void);
//   virtual Ptr<Packet> DoRemove (void);
//   virtual Ptr<const Packet> DoPeek (void) const;


    using Queue<Packet>::GetContainer;
    using Queue<Packet>::DoEnqueue;
    using Queue<Packet>::DoDequeue;
    using Queue<Packet>::DoRemove;
    using Queue<Packet>::DoPeek;

  // Classify packets based on UDP ports and place them into two different queues
  void Classify (Ptr<Packet> p);

  // Schedule packets to the outgoing link from Q1
  Ptr<Packet> Schedule ();

  Ptr<DropTailQueue<Packet>> m_queue1;
  Ptr<DropTailQueue<Packet>> m_queue2;
};

// NS_OBJECT_ENSURE_REGISTERED (TwoQueues);

TypeId
TwoQueues::GetTypeId ()
{
  static TypeId tid = TypeId ("TwoQueues")
    .SetParent<Queue<Packet>> ()
    .SetGroupName ("Network")
    .AddConstructor<TwoQueues> ()
  ;
  return tid;
}

TwoQueues::TwoQueues ()
{
  m_queue1 = CreateObject<DropTailQueue<Packet>> ();
  m_queue2 = CreateObject<DropTailQueue<Packet>> ();
}

TwoQueues::~TwoQueues ()
{
}

Ptr<Packet> TwoQueues::Remove (void)
{
  return Create<Packet> ();
}


bool TwoQueues::Enqueue(Ptr<Packet> item)
{
  return true;
}

Ptr<Packet> TwoQueues::Dequeue()
{
    Ptr<Packet> item = DoDequeue(GetContainer().begin());
    return item;
}

Ptr<const Packet> TwoQueues::Peek() const
{
    return DoPeek(GetContainer().begin());
}


// bool
// TwoQueues::DoEnqueue (Ptr<Packet> p)
// {
//   Classify (p);
//   return true;
// }

// Ptr<Packet>
// TwoQueues::DoDequeue (void)
// {
//   return Schedule ();
// }

// Ptr<Packet>
// TwoQueues::DoRemove (void)
// {
//   return 0;
// }

// Ptr<const Packet>
// TwoQueues::DoPeek (void) const
// {
//   return m_queue1->Peek ();
// }

void
TwoQueues::Classify (Ptr<Packet> p)
{
  UdpHeader udpHdr;
  p->PeekHeader (udpHdr);
  uint16_t port = udpHdr.GetDestinationPort ();

  if (port == 10000)
    {
      m_queue1->Enqueue (p);
    }
  else if (port == 20000)
    {
      m_queue2->Enqueue (p);
    }
}

Ptr<Packet>
TwoQueues::Schedule ()
{
  return m_queue1->Dequeue ();
}
