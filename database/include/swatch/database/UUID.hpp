#ifndef CACTUSCORE_SWATCH_DATABASE_UUID_H
#define CACTUSCORE_SWATCH_DATABASE_UUID_H


// SWATCH HEADERS
#include "swatch/database/HWMap.hpp"
#include "swatch/database/DBConsistency.hpp"

// OTHER HEADERS
// C++ HEADERS
#include <string>
#include <stdint.h>
#include <sstream>
#include <boost/shared_ptr.hpp>

using namespace std;

namespace swatch
{

namespace database
{
    
class UUID
{
public:
 
  UUID(uint32_t family);  
  UUID(const std::string logicid);
  UUID(uint32_t family, const std::string logicid);
  UUID(uint32_t family, uint32_t hwid);
	
 // UUID(std::string logicid, uint32_t hwid);
 // UUID(std::string logicid, uint32_t SystemID, uint32_t SubSysID, uint32_t CrateN, uint32_t processorN, uint32_t ProtN);
  //UUID( const std::string& system = "*", const std::string& crate = "*", const std::string& type ="*", uint32_t slot = 0xffff, uint32_t port = 0xffff);
  virtual ~UUID();
  
  //bool operator<(const UUID&);
  
 
  std::string LogicID();
  std::string fastLogicID();
  uint32_t HWID(){return UUID_;}
	
  uint32_t DeviceType(); 
  std::string DeviceTypeName();
  
  uint32_t processorNumber();
  uint32_t PortNumber();
  
  uint32_t CrateNumber();
  std::string CrateName();
  std::string SystemName();
  UUID SystemUUID(); 
  uint32_t family(){return uiUUIDFamily_;}
  
  bool setSystemName(const std::string&);
  bool setDeviceName(const std::string&);
  bool setDeviceTypeName(const std::string&){return true;}
  bool setCrateName(const std::string&){return true;}
  bool setSoftwareType(const std::string&){return true;}
  bool setCrateNumber(uint32_t){return true;}
  bool setDeviceNumber(uint32_t){return true;}
  bool setSlotNumber(uint32_t){return true;}
  bool setPortNumber(uint32_t){return true;}
  bool setUUIDFamily(uint32_t){return true;} 

  bool setProcessorName(const std::string&);
    
    
 
  bool operator<( const UUID& a) const;
  bool operator==( const UUID& a) const;
  
  static uint32_t getoRSCType(){return 0x1<<24 & getSysCaloLay1();}
  static uint32_t getMP7Type(){return 0x0<<24 & getSysCaloLay2();}
  static uint32_t getCTP7Type(){return 0x0<<24 & getSysCaloLay1();}
  static uint32_t getAMC13Type(){return 0x6 << 24;}
  static uint32_t getMCHType(){return 0x7<<24;}      
  static uint32_t getSysCaloLay1(){return 0x1<<28;}
  static uint32_t getSysCaloLay2(){return 0x2<<28;}
  static uint32_t familyProcessors(){return 0;}
  static uint32_t familyServices(){return 1;}
  static uint32_t familySystem(){return 2;}
  static uint32_t familyPort(){return 3;}	
private:
	
        void convertHWIDtoLID(){}
        void convertLIDtoHWID(){}
        void decodeLID(){}
        
        void decodeProcName(const std::string&);
        
        DBConsistency DBC_;
        HWMap HWMap_;
        
        uint32_t UUID_;
	//std::string LogicID_;
       
        uint32_t uiUUIDFamily_; 
        std::string sSystem_;
        std::string sDName_;   //not needed it is the combination of sysem type and DeviceNumber
        std::string sType_;
        std::string sCrateName_;
        
        uint32_t uiCrateNumber_;
        uint32_t uiDeviceNumber_;
        
        uint32_t uiSlotNumber_;
        uint32_t uiPortNumber_;
        
        std::string sSWType_;
	
     /*

        4 bits: System ID
        4 bit: (Device Type) Sub-system ID
        8 bit: Crate ID
        4 bits: Board ID
        4 bit: Software Reserved (to be left at 0 for board identification)
        8 bits: Port Number 
    */ 
};



} // end ns core
} // end ns swatch

#endif



