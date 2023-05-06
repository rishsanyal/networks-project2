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

NS_LOG_COMPONENT_DEFINE("DemoFinal");

void testSourceIPAdress(Ptr<Packet> p1){
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
    std::cout << (f1->match(p1) == 0) << std::endl;
}

void testFilterFail(Ptr<Packet> p1){
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

void testFilterPass(Ptr<Packet> p1){
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

int main (int argc, char *argv[])
{
    CommandLine cmd (__FILE__);
    cmd.Parse (argc, argv);

    Ptr<Packet> p1 = Create<Packet> (100);

    testSourceIPAdress(p1);
    testFilterFail(Create<Packet> (100));
    testFilterPass(Create<Packet> (100));

  return 0;
}