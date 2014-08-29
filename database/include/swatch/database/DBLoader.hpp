#ifndef DBLOADER_HPP
#define	DBLOADER_HPP


#include <cstdlib>
#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "swatch/database/UUID.hpp"
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Object.hpp"

#include "swatch/database/DBSnapshot.hpp"

using boost::property_tree::ptree;
using boost::property_tree::json_parser::read_json;
using namespace boost::assign;
     



using namespace std;

namespace swatch
{

namespace database
{

using swatch::core::ParameterSet;    

class DBLoader {
public:
    DBLoader(DBSnapshot&);
    virtual ~DBLoader();
    
    void loadDBFromFile(std::string&);
private:
    
    void loadProcessorsFromFile(UUID, ptree::value_type &);
    void loadServicesFromFile(UUID, ptree::value_type &);
    void loadPortsFromFile(UUID, ptree::value_type & );
    DBSnapshot& DB_;
    
    
    
};
}
}
#endif	/* DBFILLER_HPP */

