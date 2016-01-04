#include "swatch/core/ReadOnlyXParameterSet.hpp"

// boost headers
#include <boost/foreach.hpp>

// XDAQ Headers
#include "xdata/String.h"
#include "swatch/core/ReadWriteXParameterSet.hpp"


namespace swatch {
namespace core {


//---
ReadOnlyXParameterSet::ReadOnlyXParameterSet() : 
  mEntries()
{
}


//---
ReadOnlyXParameterSet::ReadOnlyXParameterSet(const ReadOnlyXParameterSet& other) :
  mEntries(other.mEntries)
{
}


//---
ReadOnlyXParameterSet::ReadOnlyXParameterSet(const XParameterSet& orig) 
{
  // FIXME: Is this the best solution for long term ?
  
  if (const ReadOnlyXParameterSet * const origPtr = dynamic_cast<ReadOnlyXParameterSet const * const>(&orig) )
  {
    *this = (*origPtr);
  }
  else if (const ReadWriteXParameterSet * const origPtr = dynamic_cast<ReadWriteXParameterSet const * const> (&orig) )
  {
    typedef ReadWriteXParameterSet::EntryMap_t::const_iterator ConstIt_t;
    for(ConstIt_t it = origPtr->mEntries.begin(); it != origPtr->mEntries.end(); it++)
      mEntries.emplace(it->first, it->second.object);
  }
  else
    throw std::runtime_error("Cannot create ReadOnlyXParameterSet from unknown XParameterSet type: " + demangleName(typeid(orig).name()));
}


//---
ReadOnlyXParameterSet::~ReadOnlyXParameterSet()
{
}


//---
bool ReadOnlyXParameterSet::operator==(const ReadOnlyXParameterSet& o) const
{
  return mEntries == o.mEntries;
}


//---
size_t ReadOnlyXParameterSet::size() const {
  return mEntries.size();
}


//---
std::set<std::string> ReadOnlyXParameterSet::keys() const
{
  std::set<std::string> names;
  std::transform(mEntries.begin(), mEntries.end(), std::inserter(names, names.end()), boost::bind(&EntryMap_t::value_type::first, _1));
  return names;
}


//---
bool ReadOnlyXParameterSet::has(const std::string& name) const {
  return mEntries.count(name);
}


//---
const xdata::Serializable&
ReadOnlyXParameterSet::get( const std::string& name ) const {
  EntryMap_t::const_iterator it = mEntries.find(name);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return *(it->second);
}


//---
const xdata::Serializable&
ReadOnlyXParameterSet::operator[](const std::string& name) const {
  return get(name);
}


//---
std::string ReadOnlyXParameterSet::parameterAsString(const std::string& name) const
{
  EntryMap_t::const_iterator it = mEntries.find(name);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound("Parameter '" + name + " not found");
  }

  return it->second->toString();
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const boost::shared_ptr<xdata::Serializable>& data )
{
  std::pair<EntryMap_t::iterator, bool> it = mEntries.emplace(name, data);

  // If failed to emplace, then throw
  if ( !it.second )
    throw XParameterExists("Parameter '" + name + "' is already defined");
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const ReadOnlyXParameterSet& otherSet)
{
  EntryMap_t::const_iterator it = otherSet.mEntries.find(name);
  
  if ( it == otherSet.mEntries.end() )
    throw XParameterNotFound("Parameter '" + name + "' does not exist in this set");

  adopt(name, it->second);
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const ReadWriteXParameterSet& otherSet)
{
  ReadWriteXParameterSet::EntryMap_t::const_iterator it = otherSet.mEntries.find(name);
  
  if ( it == otherSet.mEntries.end() )
    throw XParameterNotFound("Parameter '" + name + "' does not exist in this set");

  adopt(name, it->second.object);
}


//---
void ReadOnlyXParameterSet::erase(const std::string& name) 
{
  mEntries.erase(name);
}


} // core
} // swatch

