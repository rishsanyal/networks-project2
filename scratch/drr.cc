#include <fstream>
#include <diffserv.h>
#include <json.hpp>
#include "traffic-class.h"

using namespace ns3;

namespace ns3 {
  class DRR {
    private:
      int noOfQueues;
      vector<int> quantumValues;
      DiffServ* diffServ;
      vector<TrafficClass*> q_class;
      vector<int> deficitCounter;

    public:
      DRR(int noOfQueues, vector<int> quantumValues) {
        this->diffServ = new DiffServ(noOfQueues);
        this->noOfQueues = noOfQueues;
        this->quantumValues = quantumValues;

        // Initialize the deficit counter for each queue to 0
        for(int i = 0; i < noOfQueues; i++) {
          deficitCounter.push_back(0);
        }
      }

      bool Enqueue(const Ptr<Packet>& p) {
        // TODO: Check which queue to dequeue from.
        int index = Classify(p);
        return q_class[index]->Enqueue(p);
      }

      Ptr<Packet> Dequeue() {
        // Get the queue with non-zero deficit counter
        for(int i = 0; i < noOfQueues; i++) {
          if(!QisEmpty(i) && deficitCounter[i] > 0) {
            Ptr<Packet> packet = diffServ->Dequeue(i);
            if(packet != nullptr) {
              // Update the deficit counter for the queue
              deficitCounter[i] -= quantumValues[i];
              if(deficitCounter[i] < 0) {
                deficitCounter[i] = 0;
              }
            }
            return packet;
          }
        }

        // Get the queue with zero deficit counter
        for(int i = 0; i < noOfQueues; i++) {
          if(!QisEmpty(i) && deficitCounter[i] == 0) {
            Ptr<Packet> packet = diffServ->Dequeue(i);
            if(packet != nullptr) {
              // Update the deficit counter for the queue
              deficitCounter[i] = quantumValues[i] - packet->GetSize();
            }
            return packet;
          }
        }

        return nullptr;
      }

      uint32_t Classify(Ptr<Packet> p) {
        for(int i = 0; i < noOfQueues; i++) {
          if(q_class[i]->Match(p)) {
            return i;
          }
        }
        return 0;
      }
  };
}
