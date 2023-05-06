
// /*
//  * Copyright (c) 2007 University of Washington
//  *
//  * This program is free software; you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License version 2 as
//  * published by the Free Software Foundation;
//  *
//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.
//  *
//  * You should have received a copy of the GNU General Public License
//  * along with this program; if not, write to the Free Software
//  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//  */

// #ifndef NEWTEMPQUEUE_H
// #define NEWTEMPQUEUE_H

// #include "ns3/tcp-header.h"
// #include "ns3/udp-header.h"
// #include "ns3/queue.h"
// #include "ns3/packet.h"
// #include "ns3/command-line.h"
// #include "ns3/packet-filter.h"
// #include "ns3/queue-item.h"
// #include "ns3/ipv4-header.h"
// #include "ns3/ethernet-header.h"
// #include "ns3/log.h"

// #include <typeinfo>


// // NS_LOG_COMPONENT_DEFINE("NewTempQueue");

// using namespace std;

// namespace ns3
// {

//     // class UDPHeader;

// /**
//  * \ingroup queue
//  *
//  * \brief A FIFO packet queue that drops tail-end packets on overflow
//  */
// template <typename Item>
// class NewTempQueue : public Queue<Item>
// {
// public:
//     static TypeId GetTypeId();

//     NewTempQueue();
//     ~NewTempQueue() override;

//     bool Enqueue(Ptr<Item> item) override;
//     Ptr<Item> Dequeue() override;
//     Ptr<Item> Remove() override;
//     Ptr<const Item> Peek() const override;

// private:
//     // Additional methods
//     void Classify(Ptr<Item> item);
//     Ptr<Item> Schedule();

//     // using Queue<Item>::GetContainer;
//     // using Queue<Item>::DoEnqueue;
//     // using Queue<Item>::DoDequeue;
//     // using Queue<Item>::DoRemove;
//     // using Queue<Item>::DoPeek;


//     const list<Ptr<Item>>& GetContainer() const;
//     bool DoEnqueue (Ptr<Item> p);
//     bool DoEnqueue (list<Ptr<Item>> queue, Ptr<Item> p);
//     Ptr<Item> DoDequeue (void);
//     Ptr<Item> DoDequeue (list<Ptr<Item>> queue);
//     Ptr<Item> DoRemove (void);
//     Ptr<const Item> DoPeek (void) const;

//     // Queue<Item> m_queue1; // Queue for UDP port 10000
//     // Queue<Item> m_queue2; // Queue for UDP port 20000

//     list<Ptr<Item>> m_queue1;
//     list<Ptr<Item>> m_queue2;

//     // Ptr<PacketFilter> m_filter; // Filter to classify packets

//     NS_LOG_TEMPLATE_DECLARE; //!< redefinition of the log component
// };

// /**
//  * Implementation of the templates declared above.
//  */

// template <typename Item>
// TypeId NewTempQueue<Item>::GetTypeId()
// {
//     static TypeId tid =
//         TypeId(GetTemplateClassName<NewTempQueue<Item>>())
//             .SetParent<Queue<Item>>()
//             .SetGroupName("Network")
//             .template AddConstructor<NewTempQueue<Item>>()
//             .AddAttribute("MaxSize",
//                           "The max queue size",
//                           QueueSizeValue(QueueSize("100p")),
//                           MakeQueueSizeAccessor(&QueueBase::SetMaxSize, &QueueBase::GetMaxSize),
//                           MakeQueueSizeChecker());
//     return tid;
// }

// template <typename Item>
// NewTempQueue<Item>::NewTempQueue()
//     : Queue<Item>(),
//       NS_LOG_TEMPLATE_DEFINE("NewTempQueue")
// {
//     NS_LOG_FUNCTION(this);
//     // NS_LOG_DEBUG("NewTempQueue " << type(Item) << " created");
//     // m_filter = CreateObject<PacketFilter>();
// }

// template <typename Item>
// NewTempQueue<Item>::~NewTempQueue()
// {
//     NS_LOG_FUNCTION(this);
// }

// template <typename Item>
// const list<Ptr<Item>>&
// NewTempQueue<Item>::GetContainer() const
// {
//     NS_LOG_DEBUG("GetContainer");

//     return m_queue1;
// }

// template <typename Item>
// bool
// NewTempQueue<Item>::Enqueue(Ptr<Item> item)
// {
//     // NS_LOG_FUNCTION(this << item);
//     // NS_LOG_DEBUG("Enqueue Item");

//     cout << "Enqueue Item NewTempQueue  Item" << endl;

//     Classify(item);
//     return true;
// }

// template <>
// bool
// NewTempQueue<Packet>::Enqueue(Ptr<Packet> item)
// {
//     // NS_LOG_FUNCTION(this << item);
//     // NS_LOG_DEBUG("Enqueue Item");

//     cout << "Enqueue Item NewTempQueue Packet" << endl;

//     UdpHeader udpHeader;

//     Ipv4Header ipHeader;
//     TcpHeader tcpHeader;

//     if (item != NULL) {
//         item->PeekHeader(ipHeader);
//         item->PeekHeader(tcpHeader);
//         item->PeekHeader(udpHeader);
//     }

//     cout << "ipHeader.GetProtocol() " << ipHeader.GetProtocol() << endl;
//     cout << "tcpHeader.GetDestinationPort() " << tcpHeader.GetDestinationPort() << endl;
//     cout << "tcpHeader.GetSourcePort() " << tcpHeader.GetSourcePort() << endl;

//     cout << "udpHeader.GetDestinationPort() " << udpHeader.GetDestinationPort() << endl;
//     cout << "udpHeader.GetSourcePort() " << udpHeader.GetSourcePort() << endl;

//     // udpHeader.Print(cout);

//     // Classify(item);
//     return true;
// }

// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::Dequeue()
// {
//     NS_LOG_FUNCTION(this);

//     // Ptr<Item> item = Schedule();

//     Ptr<Item> item = m_queue1.front();
//     m_queue1.pop_front();

//     NS_LOG_LOGIC("Popped " << item);

//     return item;
// }


// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::Remove()
// {
//     NS_LOG_DEBUG("Remove");

//     Ptr<Item> item = DoRemove();

//     NS_LOG_LOGIC("Removed " << item);

//     return item;
// }

// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::DoRemove()
// {
//     NS_LOG_FUNCTION(this);

//     Ptr<Item> item = NULL;

//     NS_LOG_DEBUG("Removed " << item);

//     return item;
// }

// template <typename Item>
// Ptr<const Item>
// NewTempQueue<Item>::Peek() const
// {
//     NS_LOG_FUNCTION(this);

//     return DoPeek();
// }

// template <typename Item>
// Ptr<const Item>
// NewTempQueue<Item>::DoPeek(void) const
// {
//     NS_LOG_DEBUG("DoPeek");

//     // NS_LOG_FUNCTION(this);

//     // if (queue.empty())
//     // {
//     //     return nullptr;
//     // }

//     // Ptr<const Item> item = queue.front();

//     Ptr<Item> item = NULL;
//     return item;
// }

// template <typename Item>
// void
// NewTempQueue<Item>::Classify(Ptr<Item> item)
// {
//     // cout(this << item);
//     // Ptr<Packet> packet = item->GetPacket();
//     // UdpHeader udpHeader;
//     // item->PeekHeader(udpHeader);

//     // Ipv4Header ipv4Header;
//     // ipv4Header = item.GetHeader();

//     // ipv4Header.GetDestination();

//     // if (udpHeader.GetDestinationPort() == 10000)
//     // {
//     //     DoEnqueue(m_queue1, item);
//     // }
//     // else if (udpHeader.GetDestinationPort() == 20000)
//     // {
//     //     // DoEnqueue(m_queue2, item);
//     //     NS_LOG_DEBUG("Ignoring Packet");
//     // }

//         // Cast the Item to a Packet
//     // Ptr<Item> packet = item->Copy();

//     NS_LOG_DEBUG(typeid(item).name());
//     // const char* MY_STRING = "N3ns33PtrINS_6PacketEEE";
//     // if (strcmp(typeid(item).name(), MY_STRING) == 0){
//     //     NS_LOG_DEBUG("Packet");

//     //     UdpHeader udpHeader;
//     //     item->PeekHeader(udpHeader);

//     //     // if (udpHeader.GetDestinationPort() == 10000)
//     //     // {
//     //     //     DoEnqueue(m_queue1, item);
//     //     // }
//     //     // else if (udpHeader.GetDestinationPort() == 20000)
//     //     // {
//     //     //     // DoEnqueue(m_queue2, item);
//     //     //     NS_LOG_DEBUG("Ignoring Packet");
//     //     // }
//     // }
//     // else{
//     //     NS_LOG_DEBUG("Unknown type " << typeid(item).name() << "|" << endl);
//     // }
//     // // NS_LOG_FUNCTION("this " << this << " item " << item << " packet " << packet);
//     // NS_LOG_DEBUG("Classify");

//     if (item)
//     {

//         // Ipv4Header *ipv4Hdr;
//         // item->PeekHeader(&ipv4Hdr);


//         // Remove the Ethernet and IP headers
//         // ns3::EthernetHeader ethernetHeader;
//         // packet->RemoveHeader(ethernetHeader);

//         // ns3::Ipv4Header ipv4Header;
//         // packet->RemoveHeader(ipv4Header);

//         // // Check the protocol number to ensure it's UDP (17)
//         // if (ipv4Header.GetProtocol() == 17)
//         // {
//         //     ns3::UdpHeader udpHeader;
//         //     packet->PeekHeader(udpHeader);

//         //     // Classify based on port number
//         //     if (udpHeader.GetDestinationPort() == 10000)
//         //     {
//         //         DoEnqueue(m_queue1, item);
//         //     }
//         //     else if (udpHeader.GetDestinationPort() == 20000)
//         //     {
//         //         DoEnqueue(m_queue2, item);
//         //     }
//         //     else
//         //     {
//         //         NS_LOG_WARN("Unknown UDP destination port: " << udpHeader.GetDestinationPort());
//         //     }
//         // }
//         // else
//         // {
//         //     NS_LOG_WARN("Non-UDP packet received");
//         // }

//         // // Add the Ethernet and IP headers back
//         // packet->AddHeader(ipv4Header);
//         // packet->AddHeader(ethernetHeader);
//     }
//     // else
//     // {
//     //     NS_LOG_WARN("Failed to cast item to Packet");
//     // }

// }

// template <>
// void
// NewTempQueue<Packet>::Classify(Ptr<Packet> item)
// {
//     // UdpHeader udpHeader;
//     // item->PeekHeader(udpHeader);

//     // udpHeader.Print(std::cout);

//     item->Print(std::cout);



// }

// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::Schedule()
// {
//     NS_LOG_DEBUG("Schedule");
//     Ptr<Item> item = nullptr;

//     // FCFS scheduling logic
//     if (!m_queue1.empty())
//     {
//         item = DoDequeue(m_queue1);
//     }
//     else if (!m_queue2.empty())
//     {
//         item = DoDequeue(m_queue2);
//     }

//     return item;
// }

// template <typename Item>
// bool
// NewTempQueue<Item>::DoEnqueue(Ptr<Item> p)
// {
//     NS_LOG_DEBUG("DoEnqueue");

//     // if (Queue<Item>::DoEnqueue(p))
//     // {
//     //     NS_LOG_LOGIC("Enqueued " << p);
//     //     return true;
//     // }

//     return false;
// }

// template <typename Item>
// bool
// NewTempQueue<Item>::DoEnqueue(list<Ptr<Item>> queueList, Ptr<Item> p)
// {
//     NS_LOG_DEBUG("DoEnqueue");
//     queueList.push_back(p);
//     return true;
// }

// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::DoDequeue(void)
// {
//     NS_LOG_DEBUG("DoDequeue");

//     Ptr<Item> item = NULL;

//     NS_LOG_LOGIC("Popped " << item);

//     return item;
// }

// template <typename Item>
// Ptr<Item>
// NewTempQueue<Item>::DoDequeue(list<Ptr<Item>> queueList)
// {
//     NS_LOG_DEBUG("DoDequeue with list");
//     NS_LOG_FUNCTION(this);

//     if (queueList.empty())
//     {
//         return nullptr;
//     }

//     Ptr<Item> item = queueList.front();
//     queueList.pop_front();
//     return item;
// }


// // The following explicit template instantiation declarations prevent all the
// // translation units including this header file to implicitly instantiate the
// // NewTempQueue<Item> class and the NewTempQueue<QueueDiscItem> class. The
// // unique instances of these classes are explicitly created through the macros
// // NS_OBJECT_TEMPLATE_CLASS_DEFINE (NewTempQueue,Item) and
// // NS_OBJECT_TEMPLATE_CLASS_DEFINE (NewTempQueue,QueueDiscItem), which are included
// // in drop-tail-queue.cc
// extern template class NewTempQueue<Packet>;
// extern template class NewTempQueue<QueueDiscItem>;

// } // namespace ns3

// #endif /* NEWTEMPQUEUE_H */
