#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet.h"
#include "ns3/traffic-control-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/udp-header.h"
#include "new-spq.h"
#include "new-drr.h"
#include "filter-container.h"
#include "filter-element.h"
#include "source-ip-address.h"
#include "destination-ip-address.h"

#include "source-ip-mask.h"
#include "source-port-number.h"
#include "destination-ip-mask.h"
#include "destination-port-number.h"

using namespace ns3;
using namespace std;

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);


  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpClient", LOG_LEVEL_ALL);
  LogComponentEnable("UdpClientHelper", LOG_LEVEL_ALL);

      // NS_LOG_INFO("Create UdpServer application on node 1.");
    uint16_t port = 10000;
    uint16_t portTwo = 20000;

  // Create three nodes
  NodeContainer nodes;
  nodes.Create (3);
  Ptr<Node> n0 = nodes.Get(0);
  Ptr<Node> n1 = nodes.Get(1);
  Ptr<Node> n2 = nodes.Get(2);

  // Create point-to-point channels and set their parameters
  PointToPointHelper clientToRouter;
  clientToRouter.SetDeviceAttribute ("DataRate", StringValue ("4Mbps"));
  // clientToRouter.SetChannelAttribute ("Delay", StringValue ("20ms"));

  PointToPointHelper routerToServer;
  routerToServer.SetDeviceAttribute ("DataRate", StringValue ("1Mbps"));
  // routerToServer.SetChannelAttribute ("Delay", StringValue ("1ms"));

  // Install the channels on the nodes
  NetDeviceContainer devices1 = clientToRouter.Install (n0, n1);
  NetDeviceContainer devices2 = routerToServer.Install (n1, n2);

  // Install the InternetStack on the nodes
  InternetStackHelper stack;
  stack.InstallAll();

  // Assign IPv4 addresses to the devices
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4AddressHelper address2;
  address2.SetBase ("10.2.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  SourceIPAddress *f1 = new SourceIPAddress();
  f1->setValue(ns3::Ipv4Address("10.1.1.1"));

  SourcePortNumber *f1sp = new SourcePortNumber();
  f1sp->setValue(10000);

  DestinationPortNumber *dstPrtFltr1 = new DestinationPortNumber();
  dstPrtFltr1->setValue(port);

  DestinationPortNumber *dstPrtFltr2 = new DestinationPortNumber();
  dstPrtFltr2->setValue(portTwo);

  SourceIPMask *f1sm = new SourceIPMask();
  f1sm->setValue(Ipv4Address("10.1.1.1"), Ipv4Mask("255.255.255.0"));

  DestinationIPAddress *d1 = new DestinationIPAddress();
  d1->setValue(interfaces2.GetAddress(1));

  DestinationIPAddress *d3 = new DestinationIPAddress();
  d3->setValue(ns3::Ipv4Address("10.2.2.3"));


  DestinationIPMask *d1m = new DestinationIPMask();
  d1m->setValue(Ipv4Address("10.2.2.2"), Ipv4Mask("255.255.255.0"));


  SourceIPAddress *f2 = new SourceIPAddress();
  f2->setValue(ns3::Ipv4Address("10.1.2.2"));

  SourcePortNumber *f2sp = new SourcePortNumber();
  f2sp->setValue(20000);

  DestinationIPAddress *d2 = new DestinationIPAddress();
  d2->setValue(ns3::Ipv4Address("10.1.2.5"));

  SourceIPMask *f2sm = new SourceIPMask();
  f2sm->setValue(Ipv4Address("10.0.2.2"), Ipv4Mask("255.255.255.0"));

  DestinationIPMask *d2m = new DestinationIPMask();
  d2m->setValue(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"));

  Ipv4Address destAddress("10.2.2.2");
  DestinationIPAddress *d1dp = new DestinationIPAddress();
  d1dp->setValue(destAddress);

  DestinationIPAddress *d2dp = new DestinationIPAddress();
  d2dp->setValue(destAddress);


  // 4. Create a new Filter Container for both of those

  FilterContainer *filter1 = new FilterContainer();
  // filter1->addElement(f1);
  // filter1->addElement(d1);
  // filter1->addElement(f1sm);
  // filter1->addElement(f1sp);
  filter1->addElement(dstPrtFltr1);
  // filter1->addElement(d1m);
  // filter1->addElement(d1dp);

  FilterContainer *filter2 = new FilterContainer();
  // filter2->addElement(f2);
  // filter2->addElement(d1);
  // filter2->addElement(f2sm);
  // filter2->addElement(f2sp);
  filter2->addElement(dstPrtFltr2);
  // filter2->addElement(d2m);
  // filter1->addElement(d2dp);

  FilterContainer *filter3 = new FilterContainer();
  filter3->addElement(f1);
  filter3->addElement(d3);

  // 5. Create Traffic Class with those two filters

  NewTrafficClass *t1 = new NewTrafficClass(
      10000, 1000000, 1000000, 1, false
  );
  t1->AddFilter(filter1);

  NewTrafficClass *t2 = new NewTrafficClass(
      10000, 1000000, 1000000, 2, false
  );
  t2->AddFilter(filter2);

  NewTrafficClass *t3 = new NewTrafficClass(
      100000, 1000000, 40, 3, false
  );
  t3->AddFilter(filter3);

  // 6. Pass that Traffic class to SPQ
  Ptr<NewPriQueue> myFirstQueue = CreateObject<NewPriQueue>();
  myFirstQueue->AddTrafficClass(t1);
  myFirstQueue->AddTrafficClass(t2);
  // myFirstQueue->AddTrafficClass(t3);

  myFirstQueue->test();

  Ptr<Node> firstNode = n1;
  Ptr<PointToPointNetDevice> firstDevice = n1->GetDevice(1)->GetObject<PointToPointNetDevice>();

  firstDevice->SetQueue(myFirstQueue);
  // cout << "Debug this" << endl;

  UdpServerHelper server(port);
  UdpServerHelper serverTwo(portTwo);

  ApplicationContainer apps = server.Install(n2);
  apps.Start(Seconds(0.0));
  apps.Stop(Seconds(120.0));

  ApplicationContainer appsTwo = serverTwo.Install(n2);
  appsTwo.Start(Seconds(1.0));
  appsTwo.Stop(Seconds(120.0));

  // NS_LOG_INFO("Create UdpClient application on node 0 to send to node 1.");
  uint32_t MaxPacketSize = 1000;
  Time interPacketInterval = Seconds(0.008);
  uint32_t maxPacketCount = 400000;

  UdpClientHelper client1(interfaces2.GetAddress(1), port);
  client1.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
  client1.SetAttribute("Interval", TimeValue(interPacketInterval));
  client1.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
  ApplicationContainer tempOne = client1.Install(n0);

  tempOne.Start(Seconds(5.0));
  tempOne.Stop(Seconds(20.0));

  UdpClientHelper client2(interfaces2.GetAddress(1), portTwo);
  client2.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
  client2.SetAttribute("Interval", TimeValue(interPacketInterval));
  client2.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
  ApplicationContainer tempTwo = client2.Install(n0);

  tempTwo.Start(Seconds(1.0));
  tempTwo.Stop(Seconds(20.0));

  // // Enable generating the pcap files
  clientToRouter.EnablePcap("client-router", devices1.Get(0));
  routerToServer.EnablePcap("router-server", devices2.Get(1));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}