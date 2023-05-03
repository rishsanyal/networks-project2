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

// #ifndef TEMPQUEUE_H
// #define TEMPQUEUE_H

// #include "ns3/queue.h"
// #include "ns3/command-line.h"

// namespace ns3
// {

// /**
//  * \ingroup queue
//  *
//  * \brief A FIFO packet queue that drops tail-end packets on overflow
//  */
// template <typename Item>
// class TempQueue : public Queue<Item>
// {
//   public:
//     /**
//      * \brief Get the type ID.
//      * \return the object TypeId
//      */
//     static TypeId GetTypeId();
//     /**
//      * \brief TempQueue Constructor
//      *
//      * Creates a droptail queue with a maximum size of 100 packets by default
//      */
//     TempQueue();

//     ~TempQueue() override;

//     bool Enqueue(Ptr<Item> item) override;
//     Ptr<Item> Dequeue() override;
//     Ptr<Item> Remove() override;
//     Ptr<const Item> Peek() const override;

//   private:
//     using Queue<Item>::GetContainer;
//     using Queue<Item>::DoEnqueue;
//     using Queue<Item>::DoDequeue;
//     using Queue<Item>::DoRemove;
//     using Queue<Item>::DoPeek;

//     NS_LOG_TEMPLATE_DECLARE; //!< redefinition of the log component
// };

// /**
//  * Implementation of the templates declared above.
//  */

// template <typename Item>
// TypeId TempQueue<Item>::GetTypeId()
// {
//     static TypeId tid =
//         TypeId(GetTemplateClassName<TempQueue<Item>>())
//             .template AddConstructor<TempQueue<Item>>();
//             // .SetParent<Queue<Item>>()
//             // .SetGroupName("Network")
//             // .template AddConstructor<TempQueue<Item>>()
//             // .AddAttribute("MaxSize",
//             //               "The max queue size",
//             //               QueueSizeValue(QueueSize("100p")),
//             //               MakeQueueSizeAccessor(&QueueBase::SetMaxSize, &QueueBase::GetMaxSize),
//             //               MakeQueueSizeChecker());
//     return tid;
// }

// template <typename Item>
// TempQueue<Item>::TempQueue()
//     : Queue<Item>(),
//       NS_LOG_TEMPLATE_DEFINE("TempQueue")
// {
//     NS_LOG_FUNCTION(this);
// }

// template <typename Item>
// TempQueue<Item>::~TempQueue()
// {
//     NS_LOG_FUNCTION(this);
// }

// template <typename Item>
// bool
// TempQueue<Item>::Enqueue(Ptr<Item> item)
// {
//     NS_LOG_FUNCTION(this << item);

//     return DoEnqueue(GetContainer().end(), item);
// }

// template <typename Item>
// Ptr<Item>
// TempQueue<Item>::Dequeue()
// {
//     NS_LOG_FUNCTION(this);

//     Ptr<Item> item = DoDequeue(GetContainer().begin());

//     NS_LOG_LOGIC("Popped " << item);

//     return item;
// }

// template <typename Item>
// Ptr<Item>
// TempQueue<Item>::Remove()
// {
//     NS_LOG_FUNCTION(this);

//     Ptr<Item> item = DoRemove(GetContainer().begin());

//     NS_LOG_LOGIC("Removed " << item);

//     return item;
// }

// template <typename Item>
// Ptr<const Item>
// TempQueue<Item>::Peek() const
// {
//     NS_LOG_FUNCTION(this);

//     return DoPeek(GetContainer().begin());
// }

// // The following explicit template instantiation declarations prevent all the
// // translation units including this header file to implicitly instantiate the
// // TempQueue<Item> class and the TempQueue<QueueDiscItem> class. The
// // unique instances of these classes are explicitly created through the macros
// // NS_OBJECT_TEMPLATE_CLASS_DEFINE (TempQueue,Item) and
// // NS_OBJECT_TEMPLATE_CLASS_DEFINE (TempQueue,QueueDiscItem), which are included
// // in drop-tail-queue.cc
// extern template class TempQueue<Packet>;
// extern template class TempQueue<QueueDiscItem>;

// } // namespace ns3

// #endif /* TEMPQUEUE_H */
