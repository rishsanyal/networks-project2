/* CS621: Lab 3 Modified - Two Queues */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/udp-header.h"

#include "ns3/ppp-header.h"

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
  // void Classify (Ptr<Packet> p);

  // Schedule packets to the outgoing link from Q1
  // Ptr<Packet> Schedule ();

  list<Ptr<Packet>> m_queue1;
  list<Ptr<Packet>> m_queue2;
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
  m_queue1 = list<Ptr<Packet>> ();
  m_queue2 = list<Ptr<Packet>> ();
}

TwoQueues::~TwoQueues ()
{
}

Ptr<Packet> TwoQueues::Remove (void)
{

  std::cout << "Remove" << std::endl;
  return Create<Packet> ();
}


bool TwoQueues::Enqueue(Ptr<Packet> item)
{

  cout << "Enqueue" << endl;
  Ptr<Packet> copy = item->Copy();
  // std::cout << "Enqueue" << std::endl;
  // UdpHeader udpHdr;
  // item->PeekHeader(udpHdr);

  PppHeader pppHeader;
  copy->RemoveHeader(pppHeader);

  // cout << "Protocol: " << pppHeader.GetProtocol() << endl;
  // pppHeader.Print(std::cout);
  Ipv4Header ipHeader;
  copy->RemoveHeader(ipHeader);
  // cout << "Source IP: " << ipHeader.GetDestination() << endl;

  UdpHeader udpHdr;
  copy->PeekHeader(udpHdr);
  int port = udpHdr.GetDestinationPort();
  // udpHdr.Print(cout);

  // std::cout << "Dest Port: " << port << std::endl;

  if (port == 10000)
  {
    // std::cout << "Enqueue 1" << std::endl;
    m_queue1.push_back (item);
    return true;
  }

  return false;
}

Ptr<Packet> TwoQueues::Dequeue()
{
    // std::cout << "Dequeue" << std::endl;
    Ptr<Packet> retItem = m_queue1.front();

    if (retItem == NULL)
    {
        // std::cout << "NULL" << std::endl;
        return nullptr;
    }

    m_queue1.pop_front();

    return retItem;
}

Ptr<const Packet> TwoQueues::Peek() const
{
  std::cout << "Peek" << std::endl;
  return DoPeek(GetContainer().begin());
}

// void
// TwoQueues::Classify (Ptr<Packet> p)
// {
//   UdpHeader udpHdr;
//   p->PeekHeader (udpHdr);
//   uint16_t port = udpHdr.GetDestinationPort ();

//   std::cout << port << std::endl;

//   // if (port == 10000)
//   //   {
//   //     m_queue1->Enqueue (p);
//   //   }
//   // else if (port == 20000)
//   //   {
//   //     m_queue2->Enqueue (p);
//   //   }
// }

// Ptr<Packet>
// TwoQueues::Schedule ()
// {
//   std::cout << "Schedule" << std::endl;
//   // return m_queue1->Dequeue ();

//   return NULL;
// }