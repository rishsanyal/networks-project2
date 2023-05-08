#include <fstream>
#include <vector>
#include <utility>
#include <json.hpp>
#include "traffic-class.h"

using json = nlohmann::json;

class JsonReader : public DiffServ {
public:
  JsonReader(const std::string& config_file) {
    std::ifstream ifs(config_file);
    json config;
    ifs >> config;
    num_queues_ = config["NumQueues"].get<uint32_t>();
    priority_levels_ = config["PriorityLevel"].get<std::vector<int>>();
  }

  std::pair<std::vector<int>, uint32_t> QueueInfo() const {
    return std::make_pair(priority_levels_, num_queues_);
  }
  
private:
  uint32_t num_queues_;
  std::vector<int> priority_levels_;
};