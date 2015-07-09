#include "swatch/core/ReadWriteXParameterSet.hpp"

// boost headers
#include <boost/foreach.hpp>

// XDAQ Headers
#include "xdata/String.h"


namespace swatch {
namespace core {


//---
ReadWriteXParameterSet::ReadWriteXParameterSet() : 
  entries_()
{
}


//---
ReadWriteXParameterSet::~ReadWriteXParameterSet()
{
}


//---
bool ReadWriteXParameterSet::operator==(const ReadWriteXParameterSet& o) const
{
  return entries_ == o.entries_;
}


//---
size_t ReadWriteXParameterSet::size() const {
  return entries_.size();
}


//---
std::set<std::string> ReadWriteXParameterSet::keys() const
{
  std::set<std::string> names;
  std::transform(entries_.begin(), entries_.end(), std::inserter(names, names.end()), boost::bind(&EntryMap::value_type::first, _1));
  return names;
}


//---
bool ReadWriteXParameterSet::has(const std::string& name) const {
  return entries_.count(name);
}


//---
const xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& name ) const {
  EntryMap::const_iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return *(it->second.object);
}


//---
xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& name ) {
  EntryMap::iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return *(it->second.object);
}


//---
xdata::Serializable&
ReadWriteXParameterSet::operator[](const std::string& name) {
  return get(name);
}


//---
const xdata::Serializable&
ReadWriteXParameterSet::operator[](const std::string& name) const {
  return get(name);
}


//---
std::string ReadWriteXParameterSet::parameterAsString(const std::string& name) const {
  EntryMap::const_iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return it->second.object->toString();
}


//---
void ReadWriteXParameterSet::deepCopyFrom(const ReadWriteXParameterSet& otherSet)
{
  for( EntryMap::const_iterator it = otherSet.entries_.begin(); it != otherSet.entries_.end(); it++)
  {
    boost::shared_ptr<xdata::Serializable> clonedData( it->second.cloner(it->second.object.get()) );
    
    XEntry deepCopy(it->second.typeinfo, it->second.cloner, clonedData);
    emplace(it->first, deepCopy);
  }
}


//---
void ReadWriteXParameterSet::erase(const std::string& name) 
{
  entries_.erase(name);
}


//---
ReadWriteXParameterSet::XEntry::XEntry(const std::type_info* t, XCloner c, const boost::shared_ptr<xdata::Serializable>& s) :
  typeinfo(t),
  cloner(c),
  object(s) {
}


//---
ReadWriteXParameterSet::XEntry::XEntry(const XEntry& orig) :
  typeinfo(orig.typeinfo),
  cloner( orig.cloner),
  object( boost::shared_ptr<xdata::Serializable>( orig.object ) )
{

}


//---
bool 
ReadWriteXParameterSet::XEntry::operator==(const XEntry& other) const {

  if ( this->typeinfo != other.typeinfo ) {
    // Different type
    return false;
  } else if ( (this->object == 0x0 ) != (other.object == 0x0 )  ) {
    // One of the 2 is null
    return false;
  } else if ( (this->object == 0x0 ) && (other.object == 0x0 ) ) {
    // Both are null
    return true;
  } else {
    return this->object->equals(*other.object);
  }
}


//---
std::pair<ReadWriteXParameterSet::EntryMap::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& name, const std::type_info* t, XCloner c, const boost::shared_ptr<xdata::Serializable>& s)
{
  return entries_.emplace(name, t, c, s);
}


//---
std::pair<ReadWriteXParameterSet::EntryMap::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& name, const XEntry& entry)
{
  return entries_.emplace(name, entry);
}


} // core
} // swatch

