#ifndef DBCONSISTENCY_HPP
#define	DBCONSISTENCY_HPP

#include <string>
#include <stdint.h>
#include <sstream>

namespace swatch {
namespace database {

class DBConsistency {
public:
    DBConsistency();
    
    virtual ~DBConsistency();
    
    bool systemExists(std::string& name){return true;}
private:

};

}
}
#endif	/* DBCONSISTENCY_HPP */

