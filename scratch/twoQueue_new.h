// #include "ns3/core-module.h"
// #include "ns3/network-module.h"
// #include "ns3/internet-module.h"
// #include "ns3/point-to-point-module.h"
// #include "ns3/applications-module.h"
// #include "ns3/queue.h"
// #include "ns3/drop-tail-queue.h"
// #include "ns3/udp-header.h"

// using namespace ns3;

// class TwoQueues : public Queue<Packet>
// {
// public:
//   TwoQueues ();
//   ~TwoQueues () override;

//   bool Enqueue(Ptr<Packet> item) override;
//   Ptr<Packet> Dequeue() override;
//   Ptr<const Packet> Peek() const override;

// private:
//   using Queue<Packet>::GetContainer;
//   using Queue<Packet>::DoEnqueue;
//   using Queue<Packet>::DoDequeue;
//   using Queue<Packet>::DoPeek;

//   std::list<Ptr<Packet>> m_queue1;
// };

// static TypeId GetTypeId (void)
// {
//   static TypeId tid = TypeId ("TwoQueues")
//     .SetParent<Queue<Packet>> ()
//     .SetGroupName ("Network")
//     .AddConstructor<TwoQueues> ();
//   return tid;
// }

// // TypeId TwoQueues::GetTypeId()

// // {
// //   static TypeId tid = TypeId("TwoQueues")
// //     .SetParent<Queue<Packet>>()
// //     .SetGroupName("Network")
// //     .AddConstructor<TwoQueues>();
// //   return tid;
// // }

// TwoQueues::TwoQueues()
// {
//   m_queue1 = std::list<Ptr<Packet>>();
// }

// TwoQueues::~TwoQueues()
// {
// }

// bool TwoQueues::Enqueue(Ptr<Packet> item)
// {
//   UdpHeader udpHdr;
//   item->PeekHeader(udpHdr);
//   udpHdr.Print(std::cout);

//   m_queue1.push_back(item);

//   return true;
// }

// Ptr<Packet> TwoQueues::Dequeue()
// {
//   Ptr<Packet> retItem = m_queue1.front();
//   m_queue1.pop_front();
//   return retItem;
// }

// Ptr<const Packet> TwoQueues::Peek() const
// {
//   std::cout << "Peek" << std::endl;
//   return DoPeek(GetContainer().begin());
// }

/* CS621: Lab 3 Modified - Two Queues */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/queue.h"
#include "ns3/drop-tail-queue.h"
#include "ns3/udp-header.h"

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
  return Create<Packet> ();
}


bool TwoQueues::Enqueue(Ptr<Packet> item)
{
  // std::cout << "Enqueue" << std::endl;
  UdpHeader udpHdr;
  item->PeekHeader(udpHdr);

  // int srcPort = udpHdr.GetSourcePort();
  // int port = udpHdr.GetDestinationPort();

  udpHdr.Print(std::cout);




  m_queue1.push_back (item);

  // std::cout << "Dest Port: " << port << std::endl;
  // std::cout << "Source Port: " << srcPort << std::endl;

  // if (port == 10000)
  //   {
  //     std::cout << "First Queue" << std::endl;
  //     // m_queue1.push_back (item);
  //   }
  // else if (port == 20000)
  //   {
  //     std::cout << "Second Queue" << std::endl;
  //   }
  return true;
}

Ptr<Packet> TwoQueues::Dequeue()
{
    // Ptr<Packet> item = DoDequeue(GetContainer().begin());

    Ptr<Packet> retItem = m_queue1.front();

    m_queue1.pop_front();

    return retItem;
}

Ptr<const Packet> TwoQueues::Peek() const
{
  std::cout << "Peek" << std::endl;
  return DoPeek(GetContainer().begin());
}

// int main(int argc, char *argv[])
// {
//   CommandLine cmd;
//   cmd.Parse(argc, argv);

//   NodeContainer nodes;
//   nodes.Create(2);

//   PointToPointHelper pointToPoint;
//   pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
//   pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

//   NetDeviceContainer devices;
//   devices = pointToPoint.Install(nodes);

//   InternetStackHelper stack;
//   stack.Install(nodes);

//   Ipv4AddressHelper address;
//   address.SetBase("10.1.1.0", "255.255.255.0");

//   Ipv4InterfaceContainer interfaces = address.Assign(devices);

//   uint16_t sinkPort = 8080;
//   Address sinkAddress(InetSocketAddress(interfaces.GetAddress(1), sinkPort));
//   PacketSinkHelper sinkHelper("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), sinkPort));
//   ApplicationContainer sinkApp = sinkHelper.Install(nodes.Get(1));
//   sinkApp.Start(Seconds(0.0));
//   sinkApp.Stop(Seconds(10.0));

//   OnOffHelper onoff("ns3::UdpSocketFactory", sinkAddress);
//   onoff.SetAttribute("DataRate", StringValue("1Mbps"));
//   onoff.SetAttribute("PacketSize", UintegerValue(1000));
//   ApplicationContainer apps = onoff.Install(nodes.Get(0));
//   apps.Start(Seconds(1.0));
//   apps.Stop(Seconds(9.0));

//   Simulator::Run();
//   Simulator::Destroy();
//   return 0;
// }
