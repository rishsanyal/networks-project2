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

#include"protocol-number.cc"

#include"filter-container.h"

#include"source-ip-address.h"
#include"destination-ip-address.h"
#include"source-ip-mask.h"
#include"source-port-number.h"

#include"destination-port-number.h"
#include "destination-ip-mask.h"

#include "new-traffic-class.h"
#include "new-diffserv.h"
#include "new-spq.h"
#include "new-drr.h"

#include <string>
#include <iostream>
#include <unistd.h>

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
    ipv4Header.SetProtocol(8);

    UdpHeader udpHeader;
    udpHeader.SetSourcePort(1234);
    udpHeader.SetDestinationPort(5678);

    PppHeader pppHeader;
    pppHeader.SetProtocol(0x0021);

    p->AddHeader(udpHeader);
    p->AddHeader(ipv4Header);
    p->AddHeader(pppHeader);

    return p;
}

void testSourceIPAdress(Ptr<ns3::Packet> p1){
    SourceIPAddress *f1 = new SourceIPAddress();
    f1->setValue(Ipv4Address("10.0.1.1"));

    std::cout << "testSourceIPAdress: ";
    std::cout << (f1->match(p1) == 1) << std::endl;
}

void testSourceMask(Ptr<ns3::Packet> p1){
    SourceIPMask *f1 = new SourceIPMask();
    f1->setValue(Ipv4Address("10.0.1.1"), Ipv4Mask("255.255.1.0"));

    std::cout << "testSourceMask: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testDestinationIPAddress(Ptr<ns3::Packet> p1){

    DestinationIPAddress *f1 = new DestinationIPAddress();
    f1->setValue(Ipv4Address("10.0.2.1"));

    std::cout << "testDestinationIPAddress: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testSourcePortNumber(Ptr<ns3::Packet> p1){
    SourcePortNumber *f1 = new SourcePortNumber();
    f1->setValue(1234);

    std::cout << "testSourceIPAdress: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testDestinationPortNumber(Ptr<ns3::Packet> p1){
    DestinationPortNumber *f1 = new DestinationPortNumber();
    f1->setValue(5678);

    std::cout << "testDestinationIPAdress: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testProtocolNumber(Ptr<ns3::Packet> p1){
    ProtocolNumber *f1 = new ProtocolNumber();
    f1->setValue(8);

    std::cout << "testProtocolNumber: ";
    std::cout << (f1->match(p1) == true) << std::endl;
}

void testDestinationMask(Ptr<ns3::Packet> p1){

    DestinationIPMask *f1 = new DestinationIPMask();
    f1->setValue(Ipv4Address("10.0.2.1"), Ipv4Mask("255.255.255.0"));

    std::cout << "testDestinationMask: ";
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

    FilterContainer *filter = new FilterContainer();

    filter->addElement(f1);

    cout << "Filter Element Output: ";
    cout << (filter->match(p1) == 0) << endl;
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


    Ptr<ns3::Packet> p_default = __createPacket("p_default_2", "10.3.3.3");

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
    NewTrafficClass *t1 = new NewTrafficClass(
        10, 10000, 0.0, 2, false
    );
    t1->AddFilter(filter1);

    NewTrafficClass *t2 = new NewTrafficClass(
        10, 10000, 0.0, 1, true
    );
    t2->AddFilter(filter2);

    // 6. Pass that Traffic class to SPQ
    NewPriQueue *spq = new NewPriQueue();

    spq->AddTrafficClass(t1);
    spq->AddTrafficClass(t2);

    spq->test();

    // 7. Check if the packets are enqueued in the correct order
    // spq->Enqueue(p_1_1);
    // spq->Enqueue(p_2_1);
    // spq->Enqueue(p_1_2);
    // spq->Enqueue(p_2_2);
    // spq->Enqueue(p_2_3);
    spq->Enqueue(p_default);

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

    std::string a(6, 'a');
    std::string b(11, 'a');
    std::string c(11, 'a');

    // 1. Create Source packets with two different Applications
    // Packets for queue 1
    Ptr<ns3::Packet> p_1_1 = __createPacket(a, SOURCE_IP);
    cout << p_1_1->GetUid() << ": " << p_1_1->GetSize() << endl;

    Ptr<ns3::Packet> p_1_2 = __createPacket(b, SOURCE_IP);
    cout << p_1_2->GetUid() << ": " << p_1_2->GetSize() << endl;

    // 2. Create Destination IP Address
    // ns3::Packet for queue 2
    const char * source_ip_two = "10.2.2.2";
    Ptr<ns3::Packet> p_2_1 = __createPacket(c, source_ip_two);
    cout << p_2_1->GetUid() << ": " << p_2_1->GetSize() << endl;

    Ptr<ns3::Packet> p_2_2 = __createPacket("p_2_2", source_ip_two);
    Ptr<ns3::Packet> p_2_3 = __createPacket("p_2_3", source_ip_two);

    Ptr<ns3::Packet> p_default = __createPacket("p_default_2", "10.3.3.3");

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
    NewTrafficClass *t1 = new NewTrafficClass(
        10, 10000, 35, 0, false
    );
    t1->AddFilter(filter1);

    NewTrafficClass *t2 = new NewTrafficClass(
        10, 10000, 40, 0, true
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
    drr->Enqueue(p_default);
    drr->Enqueue(p_2_1);
    drr->Enqueue(p_1_2);
    drr->Enqueue(p_2_2);
    drr->Enqueue(p_2_3);

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

    Ptr<ns3::Packet> p1 = __createPacket("Test Message");

    std::cout << "1 means success" << std::endl;

    // testSourceIPAdress(p1);
    // testSourceMask(p1);
    // testSourcePortNumber(p1);
    // testDestinationIPAddress(p1);
    // testDestinationMask(p1);
    // testDestinationPortNumber(p1);
    // testProtocolNumber(p1);

    // testFilterFail(Create<ns3::Packet> (100));
    // testFilterPass(Create<ns3::Packet> (100));

    // TestTCPass(Create<ns3::Packet> (100));

    // testSPQ();
    testDRR();

  return 0;
}