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
// #include "temp-queue.h"
#include "dummyQueue.h"

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

  // Create three nodes
  NodeContainer nodes;
  nodes.Create (3);

  TrafficControlHelper tch;

  // Create point-to-point channels and set their parameters
  PointToPointHelper clientToRouter;
  clientToRouter.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
  clientToRouter.SetChannelAttribute ("Delay", StringValue ("20ms"));
  clientToRouter.SetQueue("TwoQueues");

//   tch.Install(nodes.Get(0), clientToRouter);

  PointToPointHelper routerToServer;
  routerToServer.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  routerToServer.SetChannelAttribute ("Delay", StringValue ("20ms"));

  // Install the channels on the nodes
  NetDeviceContainer devices1 = clientToRouter.Install (nodes.Get (0), nodes.Get (1));
  NetDeviceContainer devices2 = routerToServer.Install (nodes.Get (1), nodes.Get (2));

  // Install the InternetStack on the nodes
  InternetStackHelper stack;
  stack.Install (nodes);

  // Assign IPv4 addresses to the devices
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4AddressHelper address2;
  address2.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  // Set up the UdpEchoServer
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (2));
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

  ApplicationContainer clientApps1 = echoClient1.Install (nodes.Get (0));
  clientApps1.Start (Seconds (2.0));
  clientApps1.Stop (Seconds (10.0));

  ApplicationContainer clientApps2 = echoClient2.Install (nodes.Get (0));
  clientApps2.Start (Seconds (2.0));
  clientApps2.Stop (Seconds (10.0));

  // Enable generating the pcap files
  clientToRouter.EnablePcapAll("client-router");
  routerToServer.EnablePcapAll("router-server");

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
