#ifndef CACTUSCORE_SWATCH_CORE_SYSTEMDB_H
#define CACTUSCORE_SWATCH_CORE_SYSTEMDB_H

//using namespace std;

// SWATCH HEADERS
#include "swatch/database/DBEntry.hpp"
#include "swatch/database/UUID.hpp"

#include "swatch/processor/Port.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/database/DBSnapshot.hpp"
#include "swatch/database/DBLoader.hpp"

// OTHER HEADERS
// C++ HEADERS
#include <string>
#include <vector>
#include <stdint.h>
#include <boost/shared_ptr.hpp>

using namespace std;

namespace swatch
{

namespace database
{
    

class SystemDB{
public:
  
  //static SystemDB& Instance(){
  //{
  //	static SystemDB System;
  //	return System;
  //}
     
    
 SystemDB(); 
  ~SystemDB();
  

 
  void CheckDBConsistency();	
	
  uint32_t getNprocessors(uint32_t type); //0= oRSC, 1= MP7, 2=CTP7, -1=all
  int32_t NChannelsPerprocessor(uint32_t Type); 
  std::vector<Port> getActiveChannelsprocessor(uint32_t processorType, uint32_t processorNumber);	
  std::vector<UUID> getActiveTxUUID();
  
  std::vector<UUID> getprocessorIds(uint32_t Type);
  
  std::vector<UUID> getprocessorsInSystem(UUID);
  
  ParameterSet& getParameters(UUID);
  
  void loadDBFromFile(string&);
protected:	
  

  private:
  
   
  
//  SystemDB(const SystemDB&);
 // SystemDB& operator=(const SystemDB&);
  std::vector<DBEntry> database_;
  
  std::map<std::string, DBSnapshot> mDatabase_;
  
 
  void LoadDB();
  void DecodeDB(){;}
  
  // Local variables
};
} // end ns core
} // end ns swatch
#endif


