#include <map>

#include "swatch/database/DBSnapshot.hpp"

namespace swatch
{

namespace database
{


DBSnapshot::DBSnapshot() {
}


DBSnapshot::~DBSnapshot() {
}

ParameterSet& DBSnapshot::findInUPsMap(UUID key, UPsMap& map){
    UPsMap::iterator it =  map.find(key);
    if(it != map.end()) return it->second;
    ParameterSet ps;
    return ps; 
}

ParameterSet& DBSnapshot::findInUUPsMap(UUID key, UUPsMap& map){
    UUID SysId(UUID::familySystem()); //(key.SystemUUID());
    UUPsMap::iterator it = map.find(SysId);
    return this->findInUPsMap(key, it->second);
}

ParameterSet& DBSnapshot::getSystemPs(UUID sId){
    return this->findInUPsMap(sId, mSystem_);
}

ParameterSet& DBSnapshot::getProcessorPs(UUID pId){
    return this->findInUUPsMap(pId, mProcessors_);
}

ParameterSet& DBSnapshot::getServicePs(UUID sId){
    return this->findInUUPsMap(sId, mServices_);
}

ParameterSet& DBSnapshot::getLinkPs(UUID lId){
    return this->findInUPsMap(lId, mLinks_);
     
}

ParameterSet& DBSnapshot::getPortPs(UUID pId){
    UUID SysId(UUID::familySystem());; //(pId.SystemUUID());
    UUUPsMap::iterator it = mPorts_.find(SysId);
    return this->findInUUPsMap(pId, it->second);
}

void DBSnapshot::addSystemPs(UUID,ParameterSet){
    
}
void DBSnapshot::addProcessorPs(UUID, ParameterSet){
    
}
void DBSnapshot::addServicePs(UUID, ParameterSet){
    
}
void DBSnapshot::addLinkPs(UUID, ParameterSet){
    
}
void DBSnapshot::addPortPs(UUID, ParameterSet){
}

}
}
