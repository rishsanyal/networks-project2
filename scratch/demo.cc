/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet.h"
#include "ns3/traffic-control-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-helper.h"
// #include "ns3/drop-tail-queue.h"
#include "ns3/udp-header.h"
// #include "ns3/new-queue.h"
#include "twoQueue.h"
#include "spq.h"

// #include "src/network/utils/temp-queue.h"
// #include "ns3/temp-queue.h"
// #include "temp-queue.h"
// #include "tempQueue.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);


  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("TempQueue", LOG_LEVEL_DEBUG);

  // Create three nodes
  NodeContainer nodes;
  nodes.Create (3);
  Ptr<Node> n0 = nodes.Get(0);
  Ptr<Node> n1 = nodes.Get(1);
  Ptr<Node> n2 = nodes.Get(2);

  // Create point-to-point channels and set their parameters
  PointToPointHelper clientToRouter;
  clientToRouter.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  clientToRouter.SetChannelAttribute ("Delay", StringValue ("20ms"));

  PointToPointHelper routerToServer;
  routerToServer.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  routerToServer.SetChannelAttribute ("Delay", StringValue ("20ms"));
  // routerToServer.SetQueue("TwoQueues");


  // Ptr<PointToPointNetDevice> p2pDev = CreateObject<PointToPointNetDevice> ();
  // p2pDev->SetQueue (CreateObject<TwoQueues> ());
  // n1->AddDevice (p2pDev);

  // Ptr<SimpleNetDevice> p2pDev = CreateObject<SimpleNetDevice> ();
  // p2pDev->SetQueue (CreateObject<TwoQueues> ());
  // n1->GetDevice(1)-> (p2pDev);

  // Install the channels on the nodes
  NetDeviceContainer devices1 = clientToRouter.Install (n0, n1);
  NetDeviceContainer devices2 = routerToServer.Install (n1, n2);

  // Ptr<TempQueue<Packet>> myQueue = CreateObject<TempQueue<Packet>>();
  // Ptr<PointToPointNetDevice> device = n1->GetDevice(0)->GetObject<PointToPointNetDevice>();

  // Ptr<Node> middleNode = nodes.Get(1);
  // Ptr<PointToPointNetDevice> middleDevice = n1->GetDevice(1)->GetObject<PointToPointNetDevice>();
  // Ptr<NewTempQueue<Packet>> myQueue = CreateObject<NewTempQueue<Packet>>();
  // middleDevice->SetQueue(myQueue);

  // Install the InternetStack on the nodes
  InternetStackHelper stack;
  stack.InstallAll();

  // Set up traffic control for SPQ
  Spq spq;
  std::string configFilename = argv[1];
  spq.SetConfig (configFilename);
  spq.Install (devices.Get (1));

  // Use TrafficControlHelper to install the custom queue on devices1
  // TrafficControlHelper tch;
  // tch.SetRootQueueDisc ("TwoQueues");
  // tch.Install (devices1);

  Ptr<Node> firstNode = n1;
  Ptr<PointToPointNetDevice> firstDevice = n1->GetDevice(1)->GetObject<PointToPointNetDevice>();
  Ptr<TwoQueues> myFirstQueue = CreateObject<TwoQueues>();
  firstDevice->SetQueue(myFirstQueue);


  // Assign IPv4 addresses to the devices
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Address addy = n2->GetDevice(1)->GetAddress();
  // std::cout << addy << std::endl;

  // Ptr<Node> middleNode = n1;
  // Ptr<PointToPointNetDevice> middleDevice = n0->GetDevice(1)->GetObject<PointToPointNetDevice>();
  // Ptr<TwoQueues> myQueue = CreateObject<TwoQueues>();
  // middleDevice->SetQueue(myQueue);

  // Set up the UdpEchoServer
  UdpEchoServerHelper echoServer (10000);

  ApplicationContainer serverApps = echoServer.Install (n2);
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  // Set up the UdpEchoClient applications with different ports
  UdpEchoClientHelper echoClient1 (interfaces2.GetAddress (1), 10000);
  echoClient1.SetAttribute ("MaxPackets", UintegerValue (10000));
  echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 20000);
  echoClient2.SetAttribute ("MaxPackets", UintegerValue (10000));
  echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps1 = echoClient1.Install (n0);
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));

  ApplicationContainer clientApps2 = echoClient2.Install (n0);
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));

  echoClient1.SetFill(clientApps1.Get(0), "Hello World");

  // Enable generating the pcap files
  clientToRouter.EnablePcapAll("client-router");
  routerToServer.EnablePcapAll("router-server");

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}