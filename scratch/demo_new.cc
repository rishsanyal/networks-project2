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
#include "new-spq.h"
#include "new-drr.h"
#include "filter-container.h"
#include "filter-element.h"
#include "source-ip-address.h"
#include "destination-ip-address.h"
// #include "protocol-number.h"
// #include "destination-port-number.h"

#include "source-ip-mask.h"
#include "source-port-number.h"
#include "destination-ip-mask.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

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
using namespace rapidxml;


FilterContainer* setFilterElements(std::string sourceIP, std::string sourcePort, std::string sourceMask, 
                       std::string destIP, std::string destMask);


// NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);


  vector<FilterContainer*> filter1;
  vector<NewTrafficClass*> trafficClass;

    // Load XML file into memory
    rapidxml::file<> xmlFile("/Users/jayanarayananj/Desktop/networks-project2/scratch/config.xml");

    // Parse XML file
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    // Find root node
    xml_node<> *root_node = doc.first_node("Config");

    // Iterate over all FilterElement nodes
    xml_node<> *filterElements_node = root_node->first_node("FilterElements");
    for (xml_node<> *filterElement_node = filterElements_node->first_node("FilterElement"); 
         filterElement_node; 
         filterElement_node = filterElement_node->next_sibling("FilterElement")) 
    {
        // Extract and print data
        std::string sourceIP = filterElement_node->first_node("SourceIP")->value();
        std::string sourcePort = filterElement_node->first_node("SourcePort")->value();
        std::string sourceMask = filterElement_node->first_node("SourceMask")->value();
        std::string destIP = filterElement_node->first_node("DestIP")->value();
        std::string destPort = filterElement_node->first_node("DestPort")->value();
        std::string destMask = filterElement_node->first_node("DestMask")->value();
        std::string protocolNumber = filterElement_node->first_node("ProtocolNumber")->value();

        FilterContainer* filterTemp = setFilterElements(sourceIP, sourcePort, sourceMask, destIP, destMask);
        filter1.push_back(filterTemp);


        NS_LOG_UNCOND("FilterElement: " << sourceIP << ", " << sourcePort << ", " << sourceMask << ", " 
                      << destIP << ", " << destPort << ", " << destMask << ", " << protocolNumber);
    }

    // // Extract UDPClient details
    // xml_node<> *UDPClient_node = root_node->first_node("UDPClient");
    // uint32_t maxPacketSize = std::stoi(UDPClient_node->first_node("MaxPacketSize")->value());
    // uint32_t maxPackets = std::stoi(UDPClient_node->first_node("MaxPackets")->value());
    // double maxTime = std::stod(UDPClient_node->first_node("MaxTime")->value());

    // NS_LOG_UNCOND("UDPClient: " << maxPacketSize << ", " << maxPackets << ", " << maxTime);

    // // Extract UDPServer details
    // xml_node<> *UDPServer_node = root_node->first_node("UDPServer");
    // uint32_t portNumber = std::stoi(UDPServer_node->first_node("PortNumber")->value());

    // NS_LOG_UNCOND("UDPServer PortNumber: " << portNumber);

    
    xml_node<> *SPQ_node = root_node->first_node("SPQ");
    xml_node<> *DRR_node = root_node->first_node("DRR");
    
    if(SPQ_node) {
    // Extract SPQ details
    uint32_t numQueues = std::stoi(SPQ_node->first_node("NumQueues")->value());

    NS_LOG_UNCOND("SPQ NumQueues: " << numQueues);

    xml_node<> *priorityLevels_node = SPQ_node->first_node("PriorityLevels");
    for (xml_node<> *level_node = priorityLevels_node->first_node("Level"); 
         level_node; 
         level_node = level_node->next_sibling("Level")) 
    {
        uint32_t priority = std::stoi(level_node->value());
        NewTrafficClass *t1 = new NewTrafficClass(
            10, 10000, 0, priority, false
        );
        for(int i=0;i<filter1.size();i++) {
            t1->AddFilter(filter1[i]);
        }
        trafficClass.push_back(t1);

        NS_LOG_UNCOND("SPQ Level: " << priority);
    }
    

    } else if(DRR_node) {
     // Extract DRR details
    uint32_t DRR_numQueues = std::stoi(DRR_node->first_node("NumQueues")->value());

    NS_LOG_UNCOND("DRR NumQueues: " << DRR_numQueues);

    xml_node<> *quanta_node = DRR_node->first_node("Quanta");
    for (xml_node<> *quantum_node = quanta_node->first_node("Quantum"); 
         quantum_node; 
         quantum_node = quantum_node->next_sibling("Quantum")) 
    {
        uint32_t quantum = std::stoi(quantum_node->value());
        NewTrafficClass *t1 = new NewTrafficClass(
            10, 10000, quantum, 0, false
        );
        for(int i=0;i<filter1.size();i++) {
            t1->AddFilter(filter1[i]);
        }
        trafficClass.push_back(t1);

        NS_LOG_UNCOND("DRR Quantum: " << quantum);
    }
    }


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
  // routerToServer.SetQueue("NewPriQueue");


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
  // SPQ spq;
  // std::string configFilename = argv[1];
  // spq.SetConfig (configFilename);
  // spq.Install (devices.Get (1));

  // Use TrafficControlHelper to install the custom queue on devices1
  // TrafficControlHelper tch;
  // tch.SetRootQueueDisc ("TwoQueues");
  // tch.Install (devices1);

  // Ptr<NewPriQueue> myFirstQueue = CreateObject<NewPriQueue>();
  // firstDevice->SetQueue(myTempQueue);


  // Assign IPv4 addresses to the devices
  Ipv4AddressHelper address1;
  address1.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4AddressHelper address2;
  address2.SetBase ("10.2.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // SourceIPAddress *f1 = new SourceIPAddress();
  // f1->setValue(ns3::Ipv4Address("10.1.1.1"));

  // SourcePortNumber *f1sp = new SourcePortNumber();
  // f1sp->setValue(9);

  // SourceIPMask *f1sm = new SourceIPMask();
  // f1sm->setValue(Ipv4Address("10.1.1.1"), Ipv4Mask("255.255.255.0"));

  // DestinationIPAddress *d1 = new DestinationIPAddress();
  // d1->setValue(ns3::Ipv4Address("10.2.2.2"));


  // DestinationIPMask *d1m = new DestinationIPMask();
  // d1m->setValue(Ipv4Address("10.2.2.2"), Ipv4Mask("255.255.255.0"));


  // SourceIPAddress *f2 = new SourceIPAddress();
  // f2->setValue(ns3::Ipv4Address("10.1.2.2"));

  // SourcePortNumber *f2sp = new SourcePortNumber();
  // f2sp->setValue(9);

  // DestinationIPAddress *d2 = new DestinationIPAddress();
  // d2->setValue(ns3::Ipv4Address("10.1.2.5"));

  // SourceIPMask *f2sm = new SourceIPMask();
  // f2sm->setValue(Ipv4Address("10.0.2.2"), Ipv4Mask("255.255.255.0"));

  // DestinationIPMask *d2m = new DestinationIPMask();
  // d2m->setValue(Ipv4Address("10.1.1.0"), Ipv4Mask("255.255.255.0"));



  // 4. Create a new Filter Container for both of those

  // FilterContainer *filter1 = new FilterContainer();
  // filter1->addElement(f1);
  // filter1->addElement(d1);
  // filter1->addElement(f1sm);
  // // filter1->addElement(f1sp);
  // filter1->addElement(d1m);

  // FilterContainer *filter2 = new FilterContainer();
  // filter2->addElement(f2);
  // filter2->addElement(d2);
  // filter2->addElement(f2sm);
  // // filter2->addElement(f2sp);
  // filter2->addElement(d2m);


  // 5. Create Traffic Class with those two filters

  // uint32_t maxPackets, uint32_t maxBytes, double weight, uint32_t priorityLevel, bool isDefault
  // uint32_t maxPackets = 10, uint32_t maxBytes = 10, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false
  // NewTrafficClass *t1 = new NewTrafficClass(
  //     10, 10000, 0.0, 2, false
  // );
  // t1->AddFilter(filter1);

  // NewTrafficClass *t2 = new NewTrafficClass(
  //     10, 10000, 0.0, 1, false
  // );
  // t2->AddFilter(filter2);

  // NewTrafficClass *t1 = new NewTrafficClass(
  //     10, 10000, 35, 0, false
  // );
  // t1->AddFilter(filter1);

  // NewTrafficClass *t2 = new NewTrafficClass(
  //     10, 10000, 40, 0, false
  // );
  // t2->AddFilter(filter2);
  // 6. Pass that Traffic class to SPQ
  // ns3::NewPriQueue *myFirstQueue = new ns3::NewPriQueue();
  ns3::NewPriQueue *myFirstQueue = new ns3::NewPriQueue();

  // Ptr<NewDRRQueue> myFirstQueue = CreateObject<NewDRRQueue>();

  for (auto it = trafficClass.begin(); it != trafficClass.end(); ++it) {
      // access the element through the iterator
      NewTrafficClass* ptr = *it;
      myFirstQueue->AddTrafficClass(ptr);
      // do something with the pointer
  }

  myFirstQueue->test();

  // Ptr<TempQueue<Packet>> myTempQueue = CreateObject<TempQueue<Packet>>();
    // We tell it to make 2 queues, one w low-pri, one w high-pri

  Ptr<Node> firstNode = n1;
  Ptr<PointToPointNetDevice> firstDevice = n1->GetDevice(1)->GetObject<PointToPointNetDevice>();

  firstDevice->SetQueue(myFirstQueue);
  // cout << "Debug this" << endl;


  // Address addy = n2->GetDevice(1)->GetAddress();
  // std::cout << addy << std::endl;

  // Ptr<Node> middleNode = n1;
  // Ptr<PointToPointNetDevice> middleDevice = n0->GetDevice(1)->GetObject<PointToPointNetDevice>();
  // Ptr<TwoQueues> myQueue = CreateObject<TwoQueues>();
  // middleDevice->SetQueue(myQueue);

  // Set up the UdpEchoServer
  // UdpEchoServerHelper echoServer (10000);

  // BulkSendHelper sender("ns3::UdpSocketFactory", InetSocketAddress(interfaces1.GetAddress(0), 10000));
  // sender.SetAttribute("MaxBytes", UintegerValue(50));

  // ApplicationContainer serverApps = sender.Install (n0);
  // serverApps.Start (Seconds (1.0));
  // serverApps.Stop (Seconds (10.0));


  // // Set up the UDP receiver
  // UdpServerHelper receiver(10000);

  // ApplicationContainer receiverApp = receiver.Install(nodes.Get(2));
  // receiverApp.Start(Seconds(0.0));
  // receiverApp.Stop(Seconds(10.0));




    // NS_LOG_INFO("Create UdpServer application on node 1.");
    uint16_t port = 4000;

    UdpServerHelper server(port);

    ApplicationContainer apps = server.Install(n2);
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));

    // NS_LOG_INFO("Create UdpClient application on node 0 to send to node 1.");
    uint32_t MaxPacketSize = 1024;
    Time interPacketInterval = Seconds(0.025);
    uint32_t maxPacketCount = 10;

    UdpClientHelper client(interfaces2.GetAddress(1), port);
    client.SetAttribute("MaxPackets", UintegerValue(maxPacketCount));
    client.SetAttribute("Interval", TimeValue(interPacketInterval));
    client.SetAttribute("PacketSize", UintegerValue(MaxPacketSize));
    apps = client.Install(n0);
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));

  // PacketSinkHelper receiver("ns3::UdpSocketFactory", InetSocketAddress(interfaces2.GetAddress(1), 10000));
  // ApplicationContainer sinkApp = receiver.Install(nodes.Get(1));
  // sinkApp.Start(Seconds(0.0));
  // sinkApp.Stop(Seconds(10.0));

  // Ptr<BulkSendApplication> source = DynamicCast<BulkSendApplication>(serverApps.Get(0));
  // Ptr<PacketSink> sink = DynamicCast<PacketSink>(sinkApp.Get(0));

  // source->TraceConnectWithoutContext("Tx", MakeCallback(&BulkSendBasicTestCase::SendTx, this));
  // sink->TraceConnectWithoutContext("Rx", MakeCallback(&BulkSendBasicTestCase::ReceiveRx, this));


  // Set up the UdpEchoClient applications with different ports
  // UdpEchoClientHelper echoClient1 (interfaces2.GetAddress (1), 10000);
  // echoClient1.SetAttribute ("MaxPackets", UintegerValue (10000));
  // echoClient1.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // echoClient1.SetAttribute ("PacketSize", UintegerValue (1024));

  // UdpEchoClientHelper echoClient2 (interfaces2.GetAddress (1), 20000);
  // echoClient2.SetAttribute ("MaxPackets", UintegerValue (10000));
  // echoClient2.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  // echoClient2.SetAttribute ("PacketSize", UintegerValue (1024));

  // ApplicationContainer clientApps1 = echoClient1.Install (n0);
  // clientApps1.Start (Seconds (2.0));
  // clientApps1.Stop (Seconds (10.0));

  // ApplicationContainer clientApps2 = echoClient2.Install (n0);
  // clientApps2.Start (Seconds (2.0));
  // clientApps2.Stop (Seconds (10.0));

  // echoClient1.SetFill(clientApps1.Get(0), "Hello World");

  // // Enable generating the pcap files
  clientToRouter.EnablePcapAll("client-router");
  routerToServer.EnablePcapAll("router-server");

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}


FilterContainer* setFilterElements(std::string sourceIP, std::string sourcePort, std::string sourceMask, 
                       std::string destIP, std::string destMask) 
{
    // Create FilterElement object
    SourceIPAddress *sip = new SourceIPAddress();
    sip->setValue(Ipv4Address(sourceIP.c_str()));

    SourcePortNumber *spn = new SourcePortNumber();
    spn->setValue(std::stoi(sourcePort));
    cout << "Source Port Number: " << std::stoi(sourcePort) << endl;

    SourceIPMask *sim = new SourceIPMask();
    sim->setValue(Ipv4Address(sourceIP.c_str()), Ipv4Mask(sourceMask.c_str()));

    DestinationIPAddress *dip = new DestinationIPAddress();
    dip->setValue(Ipv4Address(destIP.c_str()));

    // DestinationPortNumber *dpn = new DestinationPortNumber();
    // dpn->setValue(std::stoi(destPort));

    DestinationIPMask *dim = new DestinationIPMask();
    dim->setValue(Ipv4Address(destIP.c_str()), Ipv4Mask(destMask.c_str()));

    // ProtocolNumber *pn = new ProtocolNumber();
    // pn->setValue(std::stoi(protocolNumber));

    FilterContainer *filter = new FilterContainer();
    filter->addElement(sip);
    filter->addElement(spn);
    filter->addElement(sim);
    filter->addElement(dip);
    // filter->addElement(dpn);
    filter->addElement(dim);
    // filter->addElement(pn);

    return filter;

}