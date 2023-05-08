<<<<<<< HEAD
#include <fstream>
#include <json.hpp>
#include "traffic-class.h"


using json = nlohmann::json;

class SPQ: public DiffServ {
public:
  SPQ(const std::string& config_file) {
    std::ifstream ifs(config_file);
    json config;
    ifs >> config;
    uint32_t num_queues = config["NumQueue"].get<uint32_t>();
    const auto& priority_levels = config["PriorityLevel"];
    bool default_queue_found = false;
    for (uint32_t i = 0; i < num_queues; ++i) {
        uint32_t priority_level = priority_levels[i].get<uint32_t>();
        bool is_default = priority_levels[i]["IsDefault"].get<bool>();
        if (is_default) {
          if (default_queue_found) {
            throw std::runtime_error("Multiple default queues found");
          }
          default_queue_found = true;
        }
        queues_.emplace_back(priority_level, is_default);
        queue_map_[priority_level] = i;
    }
    if (!default_queue_found) {
      for (auto& q : queues_) {
        if (q.priorityLevel == std::numeric_limits<uint32_t>::min()) {
          q.isDefault = true;
          default_queue_found = true;
          break;
        }
      }
      if (!default_queue_found) {
        throw std::runtime_error("No default queue found");
      }
    }
  }

  bool Enqueue(const Ptr<Packet>& p) {
    for (auto& q : queues_) {
      if (q.Match(p)) {
        if (q.Enqueue(p)) {
          if (!q.isDefault && q.priorityLevel < highest_priority_) {
            // packets with lower priority than the highest priority queue are dropped
            q.Dequeue();
          } else {
            highest_priority_ = q.priorityLevel;
          }
          return true;
        } else {
          return false;
        }
      }
    }
    // no queue matched the packet's filter, enqueue in default queue
    for (auto& q : queues_) {
      if (q.isDefault) {
        q.Enqueue(p);
        return true;
      }
    }
    // no default queue found, drop the packet
    return false;
  }

  Ptr<Packet> Dequeue() {
    for (uint32_t i = 0; i < queues_.size(); ++i) {
      auto& q = queues_[queue_map_[highest_priority_]];
      Ptr<Packet> p = q.Dequeue();
      if (p) {
        if (q.empty()) {
          // update highest_priority_ to the next non-empty queue
          for (uint32_t j = highest_priority_ + 1; j < queues_.size(); ++j) {
            if (!queues_[queue_map_[j]].empty()) {
              highest_priority_ = j;
              break;
            }
          }
        }
        return p;
      }
    }
    // all queues are empty
    return nullptr;
  }

private:
  struct TrafficClassWithDefault {
    TrafficClassWithDefault(uint32_t p, bool d) : priorityLevel(p), isDefault(d) {}
    TrafficClass queue;
    uint32_t priorityLevel;
    bool isDefault;
  };

  std::vector<TrafficClassWithDefault> queues_;
  std::unordered_map<uint32_t, uint32_t> queue_map_; // maps priority level to queue index
  uint32_t highest_priority_ = std::numeric_limits<uint32_t>::max();
};
=======
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/traffic-control-helper.h"
#include "ns3/packet-sink.h"
#include <fstream>

using namespace ns3;

class Spq : public DiffServ {
public:
  Spq () : DiffServ () {}

  void SetConfig (const std::string& filename) {
    std::ifstream configFile (filename.c_str ());
    int numQueues;
    configFile >> numQueues;
    QueueDiscContainer queueDiscs;
    for (int i = 0; i < numQueues; i++) {
      int priorityLevel;
      configFile >> priorityLevel;
      Ptr<QueueDisc> queueDisc = CreateObject<PfifoFastQueueDisc> ();
      queueDisc->SetPriority (priorityLevel);
      queueDiscs.Add (queueDisc);
    }
    SetQueueDiscs (queueDiscs);
  }
};

// int main (int argc, char *argv[])
// {
//   // Create nodes
//   NodeContainer nodes;
//   nodes.Create (2);

//   // Create Point-to-Point link
//   PointToPointHelper pointToPoint;
//   pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
//   pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

//   NetDeviceContainer devices;
//   devices = pointToPoint.Install (nodes);

//   // Install internet stack on nodes
//   InternetStackHelper internet;
//   internet.Install (nodes);

//   // Set up traffic control for SPQ
//   Spq spq;
//   std::string configFilename = argv[1];
//   spq.SetConfig (configFilename);
//   spq.Install (devices.Get (1));

//   // Install PacketSink application on receiver node
//   Ptr<Node> receiver = nodes.Get (1);
//   uint16_t sinkPort = 8080;
//   PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory",
//                                       InetSocketAddress (Ipv4Address::GetAny (), sinkPort));
//   ApplicationContainer sinkApps = packetSinkHelper.Install (receiver);
//   sinkApps.Start (Seconds (0.0));

//   // Start simulation
//   Simulator::Run ();
//   Simulator::Destroy ();

//   return 0;
// }
>>>>>>> 65351c330 (json reader)
