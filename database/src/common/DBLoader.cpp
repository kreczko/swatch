#include "swatch/database/DBLoader.hpp"



namespace swatch
{

namespace database
{

DBLoader::DBLoader(DBSnapshot& DB): DB_(DB)
{
}


DBLoader::~DBLoader() {
}

 void DBLoader::loadDBFromFile(std::string& fName){
    
    DB_.loadFileName(fName);
    ptree pt;
    
    boost::property_tree::json_parser::read_json(fName, pt);
    
     BOOST_FOREACH(ptree::value_type &gsystem, pt.get_child("GSYSTEM")){ 
        BOOST_FOREACH(ptree::value_type &system, gsystem.second) {  
            UUID SysId(UUID::familySystem());            
            if(SysId.setSystemName(system.second.get<std::string>("NAME"))){
            ParameterSet Ps;
            Ps.insert("NAME", SysId);
            DB_.addSystemPs(SysId, Ps);
            loadProcessorsFromFile(SysId, system);
            loadServicesFromFile(SysId, system);
            } else {
                std::cout << "System " <<  system.second.get<std::string>("NAME") << " not expected" << std::endl;
            }
        }
     }
    
   /*
    ptree &system = pt.get_child("SYSTEM");
    BOOST_FOREACH(ptree::value_type &v, system.get_child("PROCESSORS")) {
    */    
     
}
 
void DBLoader::loadProcessorsFromFile(UUID SysId, ptree::value_type &system){
    std::vector<std::string> procStrArgs = list_of("PROCESSOR NAME")("PROCESSOR TYPE")("ADDRESS TABLE")("CRATE ID")("CRATE LOCATION")("CRATE DESCRIPTION");
    std::vector<std::string> procIntArgs = list_of("CRATE SLOT");
                
    BOOST_FOREACH(ptree::value_type &processors, system.second.get_child("PROCESSORS")) {
        BOOST_FOREACH(ptree::value_type &processor, processors.second) {
            ParameterSet procSet;
            BOOST_FOREACH( std::string& s, procStrArgs) {
                cout << s << " : " << processor.second.get<std::string>(s) << endl;
                procSet.set(s,processor.second.get<std::string>(s));
            }
            BOOST_FOREACH( std::string& s, procIntArgs) {
                cout << s << " : " << processor.second.get<int>(s) << endl;
                procSet.set(s,processor.second.get<int>(s));
            }
            UUID ProcId(SysId);
            
            //ProcId.setDeviceName(processor.second.get<std::string>("PROCESSOR NAME"));
            ProcId.setProcessorName(processor.second.get<std::string>("PROCESSOR NAME"));
           // ProcId.setDeviceTypeName(processor.second.get<std::string>("PROCESSOR TYPE"));   //not used if I fix the processor name
            ProcId.setCrateName(processor.second.get<std::string>("CRATE ID"));
            ProcId.setSlotNumber(processor.second.get<int>("CRATE SLOT"));
            //ProcId.setDeviceNumber(procN);
            DB_.addProcessorPs(ProcId,procSet);
            this->loadPortsFromFile(ProcId, processor);         
                    
        }            
           
    }
    
}

void DBLoader::loadPortsFromFile(UUID, ptree::value_type& processor){
    uint32_t portN=0;
    BOOST_FOREACH(ptree::value_type &ports, processor.second.get_child("PORTS NAMES")){
        
    }
}
void DBLoader::loadServicesFromFile(UUID SysId, ptree::value_type & system){
    
}
 
 
}
}