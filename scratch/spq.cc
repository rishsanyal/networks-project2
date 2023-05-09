#include <fstream>
#include<diffserv.h>
#include <json.hpp>
#include "traffic-class.h"


// using json = nlohmann::json;

using namespace ns3;

namespace ns3 {
  class SPQ: public DiffServ {

    private:
    int noOfQueues;
    vector<int> priorityLevels;
    DiffServ* diffServ;
    vector<TrafficClass*> q_class;

    public:
      SPQ() {
        q_class = GetQClass();
        // has to call the constructor of the base class
        this->diffServ = new DiffServ(noOfQueues, priorityLevels);
        this->noOfQueues = 3;
        this->priorityLevels = {1, 2, 3};
        std::sort(priorityLevels.begin(), priorityLevels.end(), [](int a, int b) {
          return a > b;
        });

      }

      bool Enqueue(const Ptr<Packet>& p) {
        // TODO: Check which queue to dequeue from.
              // Unsure what the queue should be.
              int index = this->Classify(p);
              return q_class[index]->Enqueue(p);
      }

      Ptr<Packet> Dequeue() {

        // TODO: Check which queue to dequeue from.
              // Unsure what the queue should be.
              for(int i = 0; i < this->q_class.size(); i++){
                  if(this->q_class[i]->GetPriority() == priority){
                      return q_class[i]->Dequeue();
                  }
              }
        return nullptr;
      }

      //getter function for q_class
      bool QisEmpty(int priority){
          for(int i = 0; i < this->q_class.size(); i++){
              if(this->q_class[i]->GetPriority() == priority){
                  if(this->q_class[i]->IsEmpty()){
                      return true;
                  } else {
                      return false;
                  }
              }
          }
          return NULL;
      }


      Ptr<Packet> SPQ::Schedule() {
        for(int i = 0; i < noOfQueues; i++) {
          if(!diffServ->QisEmpty(priorityLevels[i])) {
            return diffServ->Dequeue(priorityLevels[i]);
          }
        }
        return nullptr;
      }

      uint32_t Classify(Ptr<Packet> p){
              // TODO: Understand what the schdule method is used for.
              for(int i = 0; i < this->q_class.size(); i++){
                  if(this->q_class[i]->Match(p)){
                      return i;
                  }
              }

              return 0;
          }

  };
}
