#include "swatch/core/ReadWriteXParameterSet.hpp"

// boost headers
#include <boost/foreach.hpp>

// XDAQ Headers
#include "xdata/String.h"


namespace swatch {
namespace core {


//---
ReadWriteXParameterSet::ReadWriteXParameterSet() : 
  mEntries()
{
}


//---
ReadWriteXParameterSet::~ReadWriteXParameterSet()
{
}


//---
bool ReadWriteXParameterSet::operator==(const ReadWriteXParameterSet& o) const
{
  return mEntries == o.mEntries;
}


//---
size_t ReadWriteXParameterSet::size() const {
  return mEntries.size();
}


//---
std::set<std::string> ReadWriteXParameterSet::keys() const
{
  std::set<std::string> names;
  std::transform(mEntries.begin(), mEntries.end(), std::inserter(names, names.end()), boost::bind(&EntryMap_t::value_type::first, _1));
  return names;
}


//---
bool ReadWriteXParameterSet::has(const std::string& name) const {
  return mEntries.count(name);
}


//---
const xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& name ) const {
  EntryMap_t::const_iterator it = mEntries.find(name);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return *(it->second.object);
}


//---
xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& name ) {
  EntryMap_t::iterator it = mEntries.find(name);
  if ( it == mEntries.end() ) {
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
  EntryMap_t::const_iterator it = mEntries.find(name);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return it->second.object->toString();
}


//---
void ReadWriteXParameterSet::deepCopyFrom(const ReadWriteXParameterSet& otherSet)
{
  for( EntryMap_t::const_iterator it = otherSet.mEntries.begin(); it != otherSet.mEntries.end(); it++)
  {
    boost::shared_ptr<xdata::Serializable> clonedData( it->second.cloner(it->second.object.get()) );
    
    XEntry deepCopy(it->second.typeinfo, it->second.cloner, clonedData);
    emplace(it->first, deepCopy);
  }
}


//---
void ReadWriteXParameterSet::erase(const std::string& name) 
{
  mEntries.erase(name);
}


//---
ReadWriteXParameterSet::XEntry::XEntry(const std::type_info* t, XCloner_t c, const boost::shared_ptr<xdata::Serializable>& s) :
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
std::pair<ReadWriteXParameterSet::EntryMap_t::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& name, const std::type_info* t, XCloner_t c, const boost::shared_ptr<xdata::Serializable>& s)
{
  return mEntries.emplace(name, t, c, s);
}


//---
std::pair<ReadWriteXParameterSet::EntryMap_t::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& name, const XEntry& entry)
{
  return mEntries.emplace(name, entry);
}


} // core
} // swatch

