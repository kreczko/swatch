#include "swatch/core/XParameterSet.hpp"

// C++ headers
#include <typeinfo>

// Boost headers
#include "boost/foreach.hpp"

// XDAQ headers
#include "xdata/String.h"


namespace swatch {
namespace core {


//---
XParameterSet::XParameterSet() {}


//---
XParameterSet::XParameterSet(const XParameterSet& other) {}


//---
XParameterSet::~XParameterSet() {}


//---
std::ostream& operator<< ( std::ostream& aStr , const swatch::core::XParameterSet& aXParameterSet )
{
  std::set<std::string> lKeys( aXParameterSet.keys() );

  aStr << '{';
  BOOST_FOREACH( const std::string& name, lKeys ) {
    const xdata::Serializable& lData = aXParameterSet.get(name);
    
    if (name != *lKeys.begin())
      aStr << ',';

    aStr << "\"" << name << "\":(<" << demangleName(typeid(lData).name()) << ">, ";

    if ( typeid(lData) == typeid(xdata::String) ) {
      aStr << "\"" << aXParameterSet.parameterAsString(name) << "\"";
    } else {
      aStr << aXParameterSet.parameterAsString(name);
    }
    aStr << ")";
  }

  aStr << '}';
  
  return aStr;
}

} // core
} // swatch

