// SWATCH HEADERS
#include "swatch/database/UUID.hpp"


using namespace std;

namespace swatch {
namespace database {

UUID::UUID(uint32_t family){
    uiUUIDFamily_=family;
    UUID_=0xFFFFFFFF;
    
    
    sSystem_ = "*";
    sType_ ="*";
    sCrateName_ ="*";
    sDName_="*";    
    uiCrateNumber_= 0xFFFFFFFF;
    uiDeviceNumber_= 0xFFFFFFFF;
        
    uiSlotNumber_= 0xFFFFFFFF;
    uiPortNumber_= 0xFFFFFFFF;
        
    sSWType_="*";
}

UUID::UUID(const std::string logicid) {
       uiUUIDFamily_=0xFFFFFFFF;
       this->decodeLID();
    this->convertLIDtoHWID();
}

UUID::UUID(uint32_t family, const std::string logicid) {
    uiUUIDFamily_=family;
    this->decodeLID();
    this->convertLIDtoHWID();
}


UUID::UUID(uint32_t family, uint32_t hwid) {
    uiUUIDFamily_=family;
    UUID_ = hwid; 
    this->convertHWIDtoLID();
}

//UUID::UUID(std::string logicid, uint32_t SystemID, uint32_t SubSysID, uint32_t CrateN, uint32_t processorN, uint32_t ProtN) {

//}

//UUID::UUID(std::string logicid, uint32_t hwid) {
//}

UUID::~UUID() {

}

std::string UUID::LogicID(){ 
        std::stringstream ss;
        ss << "L_" << sSystem_ << "_" << sCrateName_ << "_" << uiSlotNumber_ << "_" << sType_ << "_" << uiDeviceNumber_  << "_" << uiPortNumber_ <<"_" << sSWType_ ;
        return ss.str();
}


std::string UUID::fastLogicID(){
    std::stringstream ss;
    ss << "S_" << sSystem_ << "_" << sType_ << "_" << uiDeviceNumber_  << "_" << uiPortNumber_ << "_" << sSWType_ ;
    return ss.str();

}

bool UUID::setSystemName(const std::string& name){
   // if(DBC_.systemExists(name)){
        sSystem_=name;
        
        return true;
    //}
    //return false;
}
bool UUID::setDeviceName(const std::string& name){
     //if(DBC_.deviceExists(name)){
         
        sDName_ = name;
        if(uiUUIDFamily_==this->familyProcessors()) decodeProcName(sDName_);
        return true;
     //}
     //return false;
 }

bool UUID::operator<(const UUID& a ) const {
    return true;
}

bool UUID::setProcessorName(const std::string& name){
    uiUUIDFamily_=this->familyProcessors();
    //if(DBC_.deviceExists(name)){
    sDName_ = name;
    this->decodeProcName(sDName_);
    return true;
    //}
    //return false;
}
void UUID::decodeProcName(const std::string& pname){
    uint32_t ftokenpos = pname.find("#");
    uint32_t stokenpos = pname.find("_");
    sType_ = pname.substr(0, ftokenpos);
    std::istringstream(pname.substr(ftokenpos+1,  stokenpos-ftokenpos-1 )) >> uiDeviceNumber_ ;
    sSystem_ = pname.substr(stokenpos+1);
    
}

UUID CrateName(){
    
    return UUID(0);
}

UUID SystemUUID(){
    return UUID(0);
}

uint32_t UUID::DeviceType() {
    return 0;
}

uint32_t UUID::processorNumber() {
    return 0;
}

uint32_t UUID::PortNumber() {
    return 0;
}

uint32_t UUID::CrateNumber() {
    return 0;
}

}
}
