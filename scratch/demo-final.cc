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

#include"source-ip-address.cc"
#include"filter-container.cc"
#include"source-ip-mask.cc"
#include"source-port-number.cc"
#include "traffic-class.cc"
#include"destination-port-number.cc"
#include"protocol-number.cc"
#include "spq.cc"

#include "new-traffic-class.h"
#include "new-diffserv.h"
#include "new-spq.h"
#include "new-drr.h"

#include <string>

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

const char* SOURCE_IP = "10.0.1.1";

Ptr<ns3::Packet> __createPacket(string msg, const char* sourceIP = SOURCE_IP){

    char* tempMessage = new char[msg.length() + 1];
    strcpy(tempMessage, msg.c_str());

    // Ptr<ns3::Packet> p = Create<ns3::Packet>("Hello World");
    Ptr<ns3::Packet> p = Create<ns3::Packet> (reinterpret_cast<const uint8_t*>(tempMessage), msg.length() + 1);

    ns3::Ipv4Header ipv4Header;
    Ipv4Address sourceAddress = Ipv4Address(sourceIP);
    sourceAddress.CombineMask(Ipv4Mask("255.255.0.0"));

    Ipv4Address destinationAddress = Ipv4Address("10.0.2.1");
    destinationAddress.CombineMask(Ipv4Mask("255.255.1.0"));

    ipv4Header.SetSource(sourceAddress); // Set the source IP address
    ipv4Header.SetDestination(destinationAddress); // Set the destination IP address

    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);

    p->AddHeader(udpHeader);
    p->AddHeader(ipv4Header);

    return p;
}

void testSourceIPAdress(Ptr<ns3::Packet> p1){
    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);
    p1->AddHeader(udpHeader);

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address("10.0.1.1"));

    std::cout << "testSourceIPAdress: ";
    std::cout << (f1->match(p1) == false) << std::endl;
}

void testSourcePortNumber(Ptr<ns3::Packet> p1){

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);
    p1->AddHeader(udpHeader);

    SourcePortNumber *f1 = new SourcePortNumber();
    f1->setValue(1234);

    std::cout << "testSourceIPAdress: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testDestinationPortNumber(Ptr<ns3::Packet> p1){

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);
    p1->AddHeader(udpHeader);

    DestinationPortNumber *f1 = new DestinationPortNumber();
    f1->setValue(5678);

    std::cout << "testDestinationIPAdress: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testProtocolNumber(Ptr<ns3::Packet> p1){

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1/300")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address
    ipv4Header.SetProtocol(8);

    p1->AddHeader(ipv4Header);

    // Problem caused when we set UDPHeader after IPv4Header

    ProtocolNumber *f1 = new ProtocolNumber();
    f1->setValue(8);

    std::cout << "testProtocolNumber: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}


void testSourceMask(Ptr<ns3::Packet> p1){
    SourceIPMask *f1 = new SourceIPMask();
    f1->setValue(Ipv4Address("10.0.1.1"), Ipv4Mask("255.255.1.0"));

    std::cout << "testSourceMask: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testFilterFail(Ptr<ns3::Packet> p1){
    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);
    p1->AddHeader(udpHeader);

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address("10.0.1.1"));

    udpHeader.SetSourcePort(1235);
    udpHeader.SetDestinationPort(5679);
    p1->AddHeader(udpHeader);

    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.2")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.3")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f2 = new SourceIPAddress();
    f2->setValue(Ipv4Address("10.0.1.1"));

    // ------------------------------

    FilterContainer *filter = new FilterContainer();

    filter->addElement(f1);
    filter->addElement(f2);

    cout << "Filter Element Output: ";
    cout << (filter->match(p1) == 0) << endl;
}

void testFilterPass(Ptr<ns3::Packet> p1){
    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);
    p1->AddHeader(udpHeader);

    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address("10.0.1.1"));

    // SourceIPAddress *f2 = new SourceIPAddress();
    // f2->setValue(Ipv4Address("10.0.1.1"));

    // ------------------------------

    FilterContainer *filter = new FilterContainer();

    filter->addElement(f1);

    cout << "Filter Element Output: ";
    cout << (filter->match(p1) == 0) << endl;
}

void TestTCPass(Ptr<ns3::Packet> p1) {
    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);

    p1->AddHeader(udpHeader);


    ns3::Ipv4Header ipv4Header;
    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.1")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.2")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address("10.0.1.1"));

    udpHeader.SetSourcePort(1235);
    udpHeader.SetDestinationPort(5679);
    p1->AddHeader(udpHeader);

    ipv4Header.SetSource(ns3::Ipv4Address("10.0.0.2")); // Set the source IP address
    ipv4Header.SetDestination(ns3::Ipv4Address("10.0.0.3")); // Set the destination IP address

    p1->AddHeader(ipv4Header);

    SourceIPAddress *f2 = new SourceIPAddress();
    f2->setValue(Ipv4Address("10.0.1.1"));

    FilterContainer *filter = new FilterContainer();

    filter->addElement(f1);
    filter->addElement(f2);

    TrafficClass *t1 = new TrafficClass(1000, 1000);
    bool temp1 = t1->Enqueue(p1);
    if(temp1) {
        std::cout << "Added 1st packet to queue" << std::endl;
    } else {
        std::cout << "Not added 1st packet to queue" << std::endl;
    }
    bool temp2 = t1->Enqueue(p1);
    if(temp2) {
        std::cout << "Added 2nd packet to queue" << std::endl;
    } else {
        std::cout << "Not added 2nd packet to queue" << std::endl;
    }
    bool temp3 = t1->Enqueue(p1);
    if(temp3) {
        std::cout << "Added 3rd packet to queue" << std::endl;
    } else {
        std::cout << "Not added 3rd packet to queue" << std::endl;
    }

    Ptr<ns3::Packet> p2 = t1->Dequeue();
    if(p2 == NULL) {
        std::cout << "No packet in queue" << std::endl;
    } else {
        std::cout << "Removed packet in queue" << std::endl;
    }
    cout << "Filter Element Output: ";
    cout << (t1->Match(p1) == 0) << endl;

}


bool testSPQ(){
    /*
        1. Create Source packets with two different Applications
        2. Create Destination IP Address
        3. Create filters for those two applications
        4. Create a new Filter Container for both of those
        5. Create Traffic Class with those two filters
        6. Pass that Traffic class to SPQ
        7. Check if the packets are enqueued in the correct order
    */

    // 1. Create Source packets with two different Applications

    // Packets for queue 1
    Ptr<ns3::Packet> p_1_1 = __createPacket("p_1_1", SOURCE_IP);
    cout << p_1_1->GetUid() << endl;

    Ptr<ns3::Packet> p_1_2 = __createPacket("p_1_2", SOURCE_IP);
    cout << p_1_2->GetUid() << endl;
    Ptr<ns3::Packet> p_1_3 = __createPacket("p_1_3", SOURCE_IP);

    // 2. Create Destination IP Address
    // ns3::Packet for queue 2
    const char * source_ip_two = "10.2.2.2";
    Ptr<ns3::Packet> p_2_1 = __createPacket("p_2_1", source_ip_two);
    cout << p_2_1->GetUid() << endl;

    Ptr<ns3::Packet> p_2_2 = __createPacket("p_2_2", source_ip_two);
    Ptr<ns3::Packet> p_2_3 = __createPacket("p_2_3", source_ip_two);

    // 3. Create filters for those two applications
    // Creating Source IP Filters for both the queues

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address(SOURCE_IP));

    SourceIPAddress *f2 = new SourceIPAddress();
    f2->setValue(Ipv4Address(source_ip_two));

    // 4. Create a new Filter Container for both of those

    FilterContainer *filter1 = new FilterContainer();
    filter1->addElement(f1);

    FilterContainer *filter2 = new FilterContainer();
    filter2->addElement(f2);


    // 5. Create Traffic Class with those two filters

    // uint32_t maxPackets, uint32_t maxBytes, double weight, uint32_t priorityLevel, bool isDefault
    // uint32_t maxPackets = 10, uint32_t maxBytes = 10, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false
    NewTrafficClass *t1 = new NewTrafficClass(
        10, 10, 0.0, 2, false
    );
    t1->AddFilter(filter1);

    NewTrafficClass *t2 = new NewTrafficClass(
        10, 10, 0.0, 2, false
    );
    t2->AddFilter(filter2);

    // 6. Pass that Traffic class to SPQ
    NewPriQueue *spq = new NewPriQueue();

    spq->AddTrafficClass(t1);
    spq->AddTrafficClass(t2);
    // Ptr<NewSPQ> myFirstQueue = CreateObject<NewSPQ>();

    spq->test();

    // 7. Check if the packets are enqueued in the correct order
    spq->Enqueue(p_1_1);
    spq->Enqueue(p_2_1);
    spq->Enqueue(p_1_2);


    // const char * source_ip_three = "10.3.2.1";
    // Ptr<ns3::Packet> p_3_1 = __createPacket("p_2_1", source_ip_three);
    // cout << p_3_1->GetUid() << endl;
    // spq->Enqueue(p_3_1);


    Ptr<ns3::Packet> p1 = spq->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


    p1 = spq->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


    p1 = spq->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


   return false;
}


bool testDRR(){
    /*
        1. Create Source packets with two different Applications
        2. Create Destination IP Address
        3. Create filters for those two applications
        4. Create a new Filter Container for both of those
        5. Create Traffic Class with those two filters
        6. Pass that Traffic class to DRR
        7. Check if the packets are enqueued in the correct order
    */


    // string temp1 = string s(n, 'a');
    // cout << temp1 << endl;

    std::string a(6, 'a');
    std::string b(11, 'a');
    std::string c(11, 'a');

    // 1. Create Source packets with two different Applications
    // Packets for queue 1
    Ptr<ns3::Packet> p_1_1 = __createPacket(a, SOURCE_IP);
    cout << p_1_1->GetUid() << ": " << p_1_1->GetSize() << endl;

    Ptr<ns3::Packet> p_1_2 = __createPacket(b, SOURCE_IP);
    cout << p_1_2->GetUid() << ": " << p_1_2->GetSize() << endl;
    // Ptr<ns3::Packet> p_1_3 = __createPacket("p_1_3", SOURCE_IP);

    // 2. Create Destination IP Address
    // ns3::Packet for queue 2
    const char * source_ip_two = "10.2.2.2";
    Ptr<ns3::Packet> p_2_1 = __createPacket(c, source_ip_two);
    cout << p_2_1->GetUid() << ": " << p_2_1->GetSize() << endl;

    // Ptr<ns3::Packet> p_2_2 = __createPacket("p_2_2", source_ip_two);
    // Ptr<ns3::Packet> p_2_3 = __createPacket("p_2_3", source_ip_two);

    // 3. Create filters for those two applications
    // Creating Source IP Filters for both the queues

    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address(SOURCE_IP));

    SourceIPAddress *f2 = new SourceIPAddress();
    f2->setValue(Ipv4Address(source_ip_two));

    // 4. Create a new Filter Container for both of those

    FilterContainer *filter1 = new FilterContainer();
    filter1->addElement(f1);

    FilterContainer *filter2 = new FilterContainer();
    filter2->addElement(f2);


    // 5. Create Traffic Class with those two filters

    // uint32_t maxPackets, uint32_t maxBytes, double weight, uint32_t priorityLevel, bool isDefault
    // uint32_t maxPackets = 10, uint32_t maxBytes = 10, double weight = 0.0, uint32_t priorityLevel = 0, bool isDefault = false
    NewTrafficClass *t1 = new NewTrafficClass(
        10, 10, 35, 0, false
    );
    t1->AddFilter(filter1);

    NewTrafficClass *t2 = new NewTrafficClass(
        10, 10, 40, 0, false
    );
    t2->AddFilter(filter2);

    // 6. Pass that Traffic class to SPQ
    NewDRRQueue *drr = new NewDRRQueue();

    drr->AddTrafficClass(t1);
    drr->AddTrafficClass(t2);
    // Ptr<NewSPQ> myFirstQueue = CreateObject<NewSPQ>();

    drr->test();

    // 7. Check if the packets are enqueued in the correct order
    drr->Enqueue(p_1_1);
    drr->Enqueue(p_2_1);
    drr->Enqueue(p_1_2);


    // const char * source_ip_three = "10.3.2.1";
    // Ptr<ns3::Packet> p_3_1 = __createPacket("p_2_1", source_ip_three);
    // cout << p_3_1->GetUid() << endl;
    // spq->Enqueue(p_3_1);


    Ptr<ns3::Packet> p1 = drr->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


    p1 = drr->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


    p1 = drr->Dequeue();
    if (p1 == NULL) {
        cout << "No packet in queue" << endl;
    } else {
        cout << p1->GetUid() << endl;
    }


   return false;
}

int main (int argc, char *argv[])
{
    CommandLine cmd (__FILE__);
    cmd.Parse (argc, argv);

    Ptr<ns3::Packet> p1 = Create<ns3::Packet> (100);

    // testSourceIPAdress(p1);
    // testFilterFail(Create<ns3::Packet> (100));
    // testFilterPass(Create<ns3::Packet> (100));
    // testSourcePortNumber(Create<ns3::Packet> (100));
    // testDestinationPortNumber(Create<ns3::Packet> (100));
    // testProtocolNumber(Create<ns3::Packet> (100));
    // TestTCPass(Create<ns3::Packet> (100));

    // testSourceMask(__createPacket());
    // TestTCPass(Create<ns3::Packet> (100));

    // testSPQ();
    testDRR();

  return 0;
}