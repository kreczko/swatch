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
bool ReadWriteXParameterSet::operator==(const ReadWriteXParameterSet& aOther) const
{
  return mEntries == aOther.mEntries;
}


//---
size_t ReadWriteXParameterSet::size() const
{
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
bool ReadWriteXParameterSet::has(const std::string& aName) const
{
  return mEntries.count(aName);
}


//---
const xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& aName ) const
{
  EntryMap_t::const_iterator it = mEntries.find(aName);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(aName +" not found");
  }

  return *(it->second.object);
}


//---
xdata::Serializable&
ReadWriteXParameterSet::get( const std::string& aName )
{
  EntryMap_t::iterator it = mEntries.find(aName);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(aName +" not found");
  }

  return *(it->second.object);
}


//---
xdata::Serializable&
ReadWriteXParameterSet::operator[](const std::string& aName)
{
  return get(aName);
}


//---
const xdata::Serializable&
ReadWriteXParameterSet::operator[](const std::string& aName) const
{
  return get(aName);
}


//---
std::string ReadWriteXParameterSet::parameterAsString(const std::string& aName) const
{
  EntryMap_t::const_iterator it = mEntries.find(aName);
  if ( it == mEntries.end() ) {
    throw XParameterNotFound(aName +" not found");
  }

  return it->second.object->toString();
}


//---
void ReadWriteXParameterSet::deepCopyFrom(const ReadWriteXParameterSet& aOtherSet)
{
  for( EntryMap_t::const_iterator it = aOtherSet.mEntries.begin(); it != aOtherSet.mEntries.end(); it++)
  {
    boost::shared_ptr<xdata::Serializable> clonedData( it->second.cloner(it->second.object.get()) );
    
    XEntry deepCopy(it->second.typeinfo, it->second.cloner, clonedData);
    emplace(it->first, deepCopy);
  }
}


//---
void ReadWriteXParameterSet::erase(const std::string& aName) 
{
  mEntries.erase(aName);
}


//---
ReadWriteXParameterSet::XEntry::XEntry(const std::type_info* aType, XCloner_t aCloner, const boost::shared_ptr<xdata::Serializable>& aData) :
  typeinfo(aType),
  cloner(aCloner),
  object(aData)
{
}


//---
ReadWriteXParameterSet::XEntry::XEntry(const XEntry& aOther) :
  typeinfo(aOther.typeinfo),
  cloner( aOther.cloner),
  object( boost::shared_ptr<xdata::Serializable>( aOther.object ) )
{
}


//---
bool 
ReadWriteXParameterSet::XEntry::operator==(const XEntry& aOther) const
{
  if ( this->typeinfo != aOther.typeinfo ) {
    // Different type
    return false;
  } else if ( (this->object == 0x0 ) != (aOther.object == 0x0 )  ) {
    // One of the 2 is null
    return false;
  } else if ( (this->object == 0x0 ) && (aOther.object == 0x0 ) ) {
    // Both are null
    return true;
  } else {
    return this->object->equals(*aOther.object);
  }
}


//---
std::pair<ReadWriteXParameterSet::EntryMap_t::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& aName, const std::type_info* aType, XCloner_t aCloner, const boost::shared_ptr<xdata::Serializable>& aData)
{
  return mEntries.emplace(aName, aType, aCloner, aData);
}


//---
std::pair<ReadWriteXParameterSet::EntryMap_t::iterator, bool>
ReadWriteXParameterSet::emplace(const std::string& aName, const XEntry& aEntry)
{
  return mEntries.emplace(aName, aEntry);
}


} // core
} // swatch

