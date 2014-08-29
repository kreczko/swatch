// SWATCH HEADERS
#include "swatch/database/DBEntry.hpp"

using namespace swatch::database;
using namespace std;

DBEntry::DBEntry(UUID TX_ID, UUID RX_ID, core::ParameterSet txPs, core::ParameterSet rxPs, bool linkStatus, uint32_t LinkType, uint32_t SubComId):
    TX_ID_(TX_ID), 
    RX_ID_(RX_ID), 
    TxPs_(txPs),
    RxPs_(rxPs),
    LinkStatus_(linkStatus), 
    LinkType_(LinkType), 
    SubComId_(SubComId)
{
}

// Destructor
DBEntry::~DBEntry(){
}

/*
bool DBEntry::isPresent(uint32_t processorType, uint32_t processorNumber){
  if((processorType==rxprocessorType() || processorType==txprocessorType()) && (processorNumber==txprocessorNumber() || processorNumber==txprocessorNumber()) ) return true;
  return false; 
}

bool DBEntry::isActive(uint32_t, uint32_t){
  if(isPresent(uint32_t processorType, uint32_t processorNumber)&& LinkStatus_==true) return true; 
  return false;
}

bool DBEntry::isTx(uint32_t processorType, uint32_t processorNumber){

}
*/
