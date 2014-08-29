#ifndef DBSNAPSHOT_HPP
#define	DBSNAPSHOT_HPP

#include "swatch/core/ParameterSet.hpp"
#include "swatch/database/UUID.hpp"

using namespace std;

namespace swatch
{

namespace database
{

  
using namespace swatch::core;

class DBSnapshot {
public:
    DBSnapshot();
    virtual ~DBSnapshot();
    
    void loadDBSnapshot();
    

    ParameterSet& getSystemPs(UUID);
    ParameterSet& getProcessorPs(UUID);
    ParameterSet& getServicePs(UUID);
    ParameterSet& getLinkPs(UUID);
    ParameterSet& getPortPs(UUID);

    void addSystemPs(UUID,ParameterSet);
    void addProcessorPs(UUID, ParameterSet);
    void addServicePs(UUID, ParameterSet);
    void addLinkPs(UUID, ParameterSet);
    void addPortPs(UUID, ParameterSet);
    
    std::string getFileName(){return fName_;};
    void loadFileName(std::string& name){fName_=name;};
     
private:

    
     
    typedef std::map<UUID, ParameterSet> UPsMap;
    typedef std::map<UUID, UPsMap> UUPsMap;
    typedef std::map<UUID, UUPsMap> UUUPsMap;
    
    UPsMap mSystem_;
    UUPsMap mProcessors_;
    UUUPsMap mPorts_;
    UUPsMap mServices_;
    UPsMap mLinks_;
    
    std::string fName_;
    
     //template<typename T> T& findInUPsMap(UUID key, UPsMap& map);
    ParameterSet& findInUPsMap(UUID key, UPsMap& map);
    ParameterSet& findInUUPsMap(UUID key, UUPsMap& map);
};

}
}
#endif	/* DBSNAPSHOT_HPP */

