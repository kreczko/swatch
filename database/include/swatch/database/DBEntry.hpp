#ifndef CACTUSCORE_SWATCH_CORE_DBENTRY_H
#define CACTUSCORE_SWATCH_CORE_DBENTRY_H



// SWATCH HEADERS
#include "swatch/database/UUID.hpp"
#include "swatch/core/ParameterSet.hpp"

// OTHER HEADERS
// C++ HEADERS
#include <string>
#include <stdint.h>
#include <boost/shared_ptr.hpp>


using namespace std;

namespace swatch
{

namespace database
{

//using namespace std;

// Included classes    

class DBEntry{
public:

  // Constructor
	DBEntry(UUID TX_ID, UUID RX_ID, core::ParameterSet txPs, core::ParameterSet rxPs, bool linkStatus, uint32_t LinkType, uint32_t SubComId);
  
  // Destructor
   virtual ~DBEntry();
  
	UUID txID(){return TX_ID_;};         // it is the equivalent of the linkID so an unique number that defines system, processor, number
	UUID rxID(){return RX_ID_;};         // it is the equivalent of the linkID so an unique number that defines system, processor, number
	bool linkStatus(){return LinkStatus_;};  //tells is the link is active
	int LinkType(){return LinkType_;};       // number that defines communication type 10 Gbps, 2, Gbps
	int SubComId(){return SubComId_;};      // Unique number that define which communication block as well which channel inside it  
    
    //static int createID(int processorType, int processorNumber, int LinkNumber);
    
	int txprocessorType();
	int txprocessorNumber();
	int txChannelNumber();
	
	int rxprocessorType();
	int rxprocessorNumber();
        int rxChannelNumber();
         
        core::ParameterSet getTxParameters();
        core::ParameterSet getRxParameters();
        
  // bool isPresent(int, int);
   //bool isActive(int, int);
  
     
  private:
	UUID TX_ID_;
	UUID RX_ID_;
	bool LinkStatus_; 
	uint32_t LinkType_; 
	uint32_t SubComId_;
        core::ParameterSet TxPs_;
        core::ParameterSet RxPs_;
  
};

} // end ns core
} // end ns swatch
#endif
