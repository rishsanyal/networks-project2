#include <fstream>
#include<diffserv.h>
#include <json.hpp>
#include "traffic-class.h"


using json = nlohmann::json;

class SPQ: public DiffServ {

  private:
  int noOfQueues;
  vector<int> priorityLevels;
  DiffServ* diffServ;

  // struct TrafficClassWithDefault {
  //   TrafficClassWithDefault(uint32_t p, bool d) : priorityLevel(p), isDefault(d) {}
  //   TrafficClass queue;
  //   uint32_t priorityLevel;
  //   bool isDefault;
  // };

  // std::vector<TrafficClassWithDefault> queues_;
  // std::unordered_map<uint32_t, uint32_t> queue_map_; // maps priority level to queue index
  // uint32_t highest_priority_ = std::numeric_limits<uint32_t>::max();
  public:
    SPQ() {

      // has to call the constructor of the base class
      
      
      this->diffServ = new DiffServ(noOfQueues, priorityLevels);
      this->noOfQueues = 3;
      this->priorityLevels = {1, 2, 3};
      std::sort(priorityLevels.begin(), priorityLevels.end(), [](int a, int b) {
        return a > b;
      });

      // std::ifstream ifs(config_file);
      // json config;
      // ifs >> config;
      // uint32_t num_queues = config["NumQueue"].get<uint32_t>();
      // const auto& priority_levels = config["PriorityLevel"];
      // bool default_queue_found = false;
      // for (uint32_t i = 0; i < num_queues; ++i) {
      //     uint32_t priority_level = priority_levels[i].get<uint32_t>();
      //     bool is_default = priority_levels[i]["IsDefault"].get<bool>();
      //     if (is_default) {
      //       if (default_queue_found) {
      //         throw std::runtime_error("Multiple default queues found");
      //       }
      //       default_queue_found = true;
      //     }
      //     queues_.emplace_back(priority_level, is_default);
      //     queue_map_[priority_level] = i;
      // }
      // if (!default_queue_found) {
      //   for (auto& q : queues_) {
      //     if (q.priorityLevel == std::numeric_limits<uint32_t>::min()) {
      //       q.isDefault = true;
      //       default_queue_found = true;
      //       break;
      //     }
      //   }
      //   if (!default_queue_found) {
      //     throw std::runtime_error("No default queue found");
      //   }
      // }
    }

    bool Enqueue(const Ptr<Packet>& p) {
      //   for (auto& q : queues_) {
      //     if (q.Match(p)) {
      //       if (q.Enqueue(p)) {
      //         if (!q.isDefault && q.priorityLevel < highest_priority_) {
      //           // packets with lower priority than the highest priority queue are dropped
      //           q.Dequeue();
      //         } else {
      //           highest_priority_ = q.priorityLevel;
      //         }
      //         return true;
      //       } else {
      //         return false;
      //       }
      //     }
      //   }
      //   // no queue matched the packet's filter, enqueue in default queue
      //   for (auto& q : queues_) {
      //     if (q.isDefault) {
      //       q.Enqueue(p);
      //       return true;
      //     }
      //   }
      //   // no default queue found, drop the packet
      //   return false;

    return diffServ->Enqueue(p);
    }

    Ptr<Packet> Dequeue() {
      // for (uint32_t i = 0; i < queues_.size(); ++i) {
      //   auto& q = queues_[queue_map_[highest_priority_]];
      //   Ptr<Packet> p = q.Dequeue();
      //   if (p) {
      //     if (q.empty()) {
      //       // update highest_priority_ to the next non-empty queue
      //       for (uint32_t j = highest_priority_ + 1; j < queues_.size(); ++j) {
      //         if (!queues_[queue_map_[j]].empty()) {
      //           highest_priority_ = j;
      //           break;
      //         }
      //       }
      //     }
      //     return p;
      //   }
      // }
      // // all queues are empty
      // return nullptr;

      return this->Schedule();
    }


    Ptr<Packet> SPQ::Schedule() {
      for(int i = 0; i < noOfQueues; i++) {
        if(!diffServ->QisEmpty(priorityLevels[i])) {
          return diffServ->Dequeue(priorityLevels[i]);
        }
      }
      return nullptr;
    } 
};





