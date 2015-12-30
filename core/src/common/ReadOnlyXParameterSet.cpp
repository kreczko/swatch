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
  entries_()
{
}


//---
ReadOnlyXParameterSet::ReadOnlyXParameterSet(const ReadOnlyXParameterSet& other) :
  entries_(other.entries_)
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
    typedef ReadWriteXParameterSet::EntryMap::const_iterator tConstIt;
    for(tConstIt it = origPtr->entries_.begin(); it != origPtr->entries_.end(); it++)
      entries_.emplace(it->first, it->second.object);
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
  return entries_ == o.entries_;
}


//---
size_t ReadOnlyXParameterSet::size() const {
  return entries_.size();
}


//---
std::set<std::string> ReadOnlyXParameterSet::keys() const
{
  std::set<std::string> names;
  std::transform(entries_.begin(), entries_.end(), std::inserter(names, names.end()), boost::bind(&EntryMap_t::value_type::first, _1));
  return names;
}


//---
bool ReadOnlyXParameterSet::has(const std::string& name) const {
  return entries_.count(name);
}


//---
const xdata::Serializable&
ReadOnlyXParameterSet::get( const std::string& name ) const {
  EntryMap_t::const_iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
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
  EntryMap_t::const_iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
    throw XParameterNotFound("Parameter '" + name + " not found");
  }

  return it->second->toString();
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const boost::shared_ptr<xdata::Serializable>& data )
{
  std::pair<EntryMap_t::iterator, bool> it = entries_.emplace(name, data);

  // If failed to emplace, then throw
  if ( !it.second )
    throw XParameterExists("Parameter '" + name + "' is already defined");
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const ReadOnlyXParameterSet& otherSet)
{
  EntryMap_t::const_iterator it = otherSet.entries_.find(name);
  
  if ( it == otherSet.entries_.end() )
    throw XParameterNotFound("Parameter '" + name + "' does not exist in this set");

  adopt(name, it->second);
}


//---
void ReadOnlyXParameterSet::adopt(const std::string& name , const ReadWriteXParameterSet& otherSet)
{
  ReadWriteXParameterSet::EntryMap::const_iterator it = otherSet.entries_.find(name);
  
  if ( it == otherSet.entries_.end() )
    throw XParameterNotFound("Parameter '" + name + "' does not exist in this set");

  adopt(name, it->second.object);
}


//---
void ReadOnlyXParameterSet::erase(const std::string& name) 
{
  entries_.erase(name);
}


} // core
} // swatch

