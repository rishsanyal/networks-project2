#include "ns3/core-module.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace ns3;
using namespace rapidxml;

FilterContainer* setFilterElements(std::string sourceIP, std::string sourcePort, std::string sourceMask, 
                       std::string destIP, std::string destPort, std::string destMask, 
                       std::string protocolNumber);



int main(int argc, char *argv[]) {

    vector<FilterContainer*> filter1;
    vector<NewTrafficClass*> trafficClass;

    // Load XML file into memory
    rapidxml::file<> xmlFile("config.xml");

    // Parse XML file
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    // Find root node
    xml_node<> *root_node = doc.first_node("Config");

    // Iterate over all FilterElement nodes
    xml_node<> *filterElements_node = root_node->first_node("FilterElements");
    for (xml_node<> *filterElement_node = filterElements_node->first_node("FilterElement"); 
         filterElement_node; 
         filterElement_node = filterElement_node->next_sibling("FilterElement")) 
    {
        // Extract and print data
        std::string sourceIP = filterElement_node->first_node("SourceIP")->value();
        std::string sourcePort = filterElement_node->first_node("SourcePort")->value();
        std::string sourceMask = filterElement_node->first_node("SourceMask")->value();
        std::string destIP = filterElement_node->first_node("DestIP")->value();
        std::string destPort = filterElement_node->first_node("DestPort")->value();
        std::string destMask = filterElement_node->first_node("DestMask")->value();
        std::string protocolNumber = filterElement_node->first_node("ProtocolNumber")->value();

        filter1.push_back(setFilterElements(sourceIP, sourcePort, sourceMask, destIP, destPort, destMask, protocolNumber));


        NS_LOG_UNCOND("FilterElement: " << sourceIP << ", " << sourcePort << ", " << sourceMask << ", " 
                      << destIP << ", " << destPort << ", " << destMask << ", " << protocolNumber);
    }

    // // Extract UDPClient details
    // xml_node<> *UDPClient_node = root_node->first_node("UDPClient");
    // uint32_t maxPacketSize = std::stoi(UDPClient_node->first_node("MaxPacketSize")->value());
    // uint32_t maxPackets = std::stoi(UDPClient_node->first_node("MaxPackets")->value());
    // double maxTime = std::stod(UDPClient_node->first_node("MaxTime")->value());

    // NS_LOG_UNCOND("UDPClient: " << maxPacketSize << ", " << maxPackets << ", " << maxTime);

    // // Extract UDPServer details
    // xml_node<> *UDPServer_node = root_node->first_node("UDPServer");
    // uint32_t portNumber = std::stoi(UDPServer_node->first_node("PortNumber")->value());

    // NS_LOG_UNCOND("UDPServer PortNumber: " << portNumber);

    
    xml_node<> *SPQ_node = root_node->first_node("SPQ");
    xml_node<> *DRR_node = root_node->first_node("DRR");
    
    if(SPQ_node) {
    // Extract SPQ details
    uint32_t numQueues = std::stoi(SPQ_node->first_node("NumQueues")->value());

    NS_LOG_UNCOND("SPQ NumQueues: " << numQueues);

    xml_node<> *priorityLevels_node = SPQ_node->first_node("PriorityLevels");
    for (xml_node<> *level_node = priorityLevels_node->first_node("Level"); 
         level_node; 
         level_node = level_node->next_sibling("Level")) 
    {
        uint32_t priority = std::stoi(level_node->value());
        NewTrafficClass *t1 = new NewTrafficClass(
            10, 10000, 0, priority, false
        );
        for(int i=0;i<filter1.size();i++) {
            t1->AddFilter(filter1[i]);
        }
        trafficClass.push_back(t1);

        NS_LOG_UNCOND("SPQ Level: " << priority);
    }
    

    } else if(DRR_node) {
     // Extract DRR details
    uint32_t DRR_numQueues = std::stoi(DRR_node->first_node("NumQueues")->value());

    NS_LOG_UNCOND("DRR NumQueues: " << DRR_numQueues);

    xml_node<> *quanta_node = DRR_node->first_node("Quanta");
    for (xml_node<> *quantum_node = quanta_node->first_node("Quantum"); 
         quantum_node; 
         quantum_node = quantum_node->next_sibling("Quantum")) 
    {
        uint32_t quantum = std::stoi(quantum_node->value());
        NewTrafficClass *t1 = new NewTrafficClass(
            10, 10000, quantum, 0, false
        );
        t1->AddFilter(filter1);
        trafficClass.push_back(t1);

        NS_LOG_UNCOND("DRR Quantum: " << quantum);
    }
    }

    return 0;
}

FilterContainer* setFilterElements(std::string sourceIP, std::string sourcePort, std::string sourceMask, 
                       std::string destIP, std::string destPort, std::string destMask, 
                       std::string protocolNumber) 
{
    // Create FilterElement object
    SourceIPAddress *sip = new SourceIPAddress();
    sip->setValue(ns3::Ipv4Address(sourceIP));

    SourcePortNumber *spn = new SourcePortNumber();
    spn->setValue(std::stoi(sourcePort));

    SourceIPMask *sim = new SourceIPMask();
    sim->setValue(Ipv4Address(sourceIP), Ipv4Mask(sourceMask));

    DestinationIPAddress *dip = new DestinationIPAddress();
    dip->setValue(ns3::Ipv4Address(destIP));

    DestinationPortNumber *dpn = new DestinationPortNumber();
    dpn->setValue(std::stoi(destPort));

    DestinationIPMask *dim = new DestinationIPMask();
    dim->setValue(Ipv4Address(destIP), Ipv4Mask(destMask));

    ProtocolNumber *pn = new ProtocolNumber();
    pn->setValue(std::stoi(protocolNumber));

    FilterContainer *filter = new FilterContainer();
    filter1->addElement(sip);
    filter1->addElement(spn);
    filter1->addElement(sim);
    filter1->addElement(dip);
    filter1->addElement(dpn);
    filter1->addElement(dim);
    filter1->addElement(pn);

    return filter;

}



