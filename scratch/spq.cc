#include <fstream>
#include "diffserv.cc"
// #include <json.hpp>
#include "traffic-class.h"


// using json = nlohmann::json;

using namespace ns3;

// namespace ns3 {
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
        // this->diffServ = new DiffServ(noOfQueues, priorityLevels);
        this->noOfQueues = 3;
        this->priorityLevels = {1, 2, 3};
        std::sort(priorityLevels.begin(), priorityLevels.end(), [](int a, int b) {
          return a > b;
        });

      }

      bool Enqueue(Ptr<ns3::Packet> p) override{
        // TODO: Check which queue to dequeue from.
              // Unsure what the queue should be.
              int index = this->Classify(p);
              return q_class[index]->Enqueue(p);
      }

      Ptr<ns3::Packet> Remove() override{
        return nullptr;
      }

      Ptr<const ns3::Packet> Peek() const override {
          return nullptr;
      }


      Ptr<ns3::Packet> Dequeue() override {

        // TODO: Hard coding the priority level for now.
              for(int i = 0; i < this->q_class.size(); i++){
                  if(this->q_class[i]->GetPriorityLevel() == 1){
                      return q_class[i]->Dequeue();
                  }
              }
        return nullptr;
      }

      //getter function for q_class
      bool QisEmpty(int priority) override{
          for(int i = 0; i < this->q_class.size(); i++){
              if(this->q_class[i]->GetPriorityLevel() == 1){
                  if(this->q_class[i]->isEmpty()){
                      return true;
                  } else {
                      return false;
                  }
              }
          }
          return false;
      }


      Ptr<ns3::Packet> Schedule(){
        for(int i = 0; i < noOfQueues; i++) {
          if(!diffServ->QisEmpty(priorityLevels[i])) {
            // return diffServ->Dequeue(priorityLevels[i]);
            return nullptr;
          }
        }
        return nullptr;
      }

      uint32_t Classify(Ptr<ns3::Packet> p) override{
              // TODO: Understand what the schdule method is used for.
              for(int i = 0; i < this->q_class.size(); i++){
                  if(this->q_class[i]->Match(p)){
                      return i;
                  }
              }

              return 0;
          }

  };
// }
