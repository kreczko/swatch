#include "swatch/core/XReferenceSet.hpp"
#include "swatch/core/Utilities.hpp"

// C++ Headers
#include <algorithm>

// Boost Headers
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/unordered/unordered_map.hpp>

// XDAQ Headers
#include "xdata/String.h"

namespace swatch {
namespace core {


//---
XReferenceSet::XReferenceSet() : 
mEntries() {

}


//---
XReferenceSet::~XReferenceSet() {
}

//---
std::set<std::string>
XReferenceSet::keys() const {
  std::set<std::string> aNames;
  std::transform(mEntries.begin(), mEntries.end(), std::inserter(aNames, aNames.end()), boost::bind(&EntryMap::value_type::first, _1));
  return aNames;
}


//---
bool XReferenceSet::has(const std::string& aName) const {
  return mEntries.count(aName);
}


//---
size_t XReferenceSet::size() const {
  return mEntries.size();
}


void XReferenceSet::add( const std::string& aName , XParameterSet& aData )
{
  if ( mEntries.count(aName) ) {
    throw XParameterExists(aName + " is already defined");
  }
  mEntries.insert( std::make_pair( aName , &aData.get( aName ) ) );
}

void XReferenceSet::add( const std::string& aName , const std::string& aParameterName , XParameterSet& aData )
{
  if ( mEntries.count(aName) ) {
    throw XParameterExists(aName + " is already defined");
  }
  mEntries.insert( std::make_pair( aName , &aData.get( aParameterName ) ) );
}

//---
xdata::Serializable&
XReferenceSet::get( const std::string& aName ) {
  EntryMap::iterator it = mEntries.find(aName);
  if ( it == mEntries.end() ) {
    throw XReferenceNotFound(aName +" not found");
  }

  return *(it->second);
}


//---
const xdata::Serializable&
XReferenceSet::get( const std::string& aName ) const {
  return get(aName);
}


//---
xdata::Serializable&
XReferenceSet::operator[](const std::string& aName) {
  return get(aName);
}


//---
//---
const xdata::Serializable&
XReferenceSet::operator[](const std::string& aName) const {
  return get(aName);
}

} // core
} // swatch

