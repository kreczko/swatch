#include "swatch/database/SystemDB.hpp"



using namespace std;
namespace swatch {
namespace database {

// Constructor
  SystemDB::SystemDB(){
    LoadDB();
    DecodeDB();
}

// Destructor
SystemDB::~SystemDB(){
}


//protected methods
void SystemDB::LoadDB(){
 //
 // DBEntry(int TX_ID, int RX_ID, bool linkStatus, int LinkType, int SubComId ):	
 //hardcoded values for the moment
 
 //oRSC0=0
 //oRSC1=1
 //MP7=2
 //CTP7=3
  core::ParameterSet txPs;
  core::ParameterSet rxPs;
  database_.clear();
  database_.push_back(DBEntry(1,2, txPs, rxPs, true,0,0));
  database_.push_back(DBEntry(1,2, txPs, rxPs, true,0,1));
  database_.push_back(DBEntry(1,3, txPs, rxPs,true,0,0));		
  database_.push_back(DBEntry(1,3, txPs, rxPs, true,0,1));
  void DecodeDB();  
}

void DecodeDb(){
}

std::vector<UUID> SystemDB::getprocessorsInSystem(UUID id){
    return this->getprocessorIds(id.DeviceType());
}

std::vector<UUID>  SystemDB::getprocessorIds(uint32_t processorType){
	std::vector<UUID> BIds;
	BIds.clear();
	
	std::vector<DBEntry>::iterator itDB;
	
	for(itDB=database_.begin(); itDB != database_.end(); ++itDB){
		UUID Rx = itDB->rxID();
		UUID Tx = itDB->txID();
   		    
	    if(Tx.DeviceType()== processorType) BIds.push_back(Tx);
	    else if(Rx.DeviceType()== processorType) BIds.push_back(Rx);
	}
	return BIds;
}
 					 
int32_t SystemDB::NChannelsPerprocessor(uint32_t processorType){
		  if(processorType == UUID::getoRSCType()) return 23;
		  if(processorType == UUID::getMP7Type()) return 72;
		  if(processorType == UUID::getCTP7Type()) return 96;
		  return -1;
}					 



std::vector<Port> SystemDB::getActiveChannelsprocessor(uint32_t processorType, uint32_t processorId){
	std::vector<Port> ActiveChannels;
	ActiveChannels.clear();
	
	
	std::vector<DBEntry>::iterator itDB;
	
	for(itDB=database_.begin(); itDB != database_.end(); ++itDB){
		//Port(int,int,int,bool,int,int,bool);  //processortype,processorId,ChannelId,isTx,LinkId,IORate,Active
			
                    UUID Rx = itDB->rxID();
		    UUID Tx = itDB->txID();
	
   	    
	        if(Tx.DeviceType()== processorType && Tx.processorNumber() == processorId && itDB->linkStatus()){
//	        	ActiveChannels.push_back(Port(Tx, Rx, true , itDB->LinkType(), true));
	        }
	        if(Rx.DeviceType()== processorType && Rx.processorNumber() && itDB->linkStatus()){
//				ActiveChannels.push_back(Port(Rx, Tx, false , itDB->LinkType(), true));
			}
	}
	
	return ActiveChannels;	

}	


std::vector<UUID> SystemDB::getActiveTxUUID(){
	std::vector<UUID> vLID;
	vLID.clear();
	
	std::vector<DBEntry>::iterator itDB;
	for(itDB=database_.begin(); itDB != database_.end(); ++itDB){
		if(itDB->linkStatus()) vLID.push_back(itDB->txID());
	}
	return vLID;
}	



ParameterSet& SystemDB::getParameters(UUID) {
    ParameterSet ps;
    return ps;
}

}
}

