#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Utilities.hpp"

// C++ Headers
#include <algorithm>

// Boost Headers
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

namespace swatch {
namespace core {


//---
XParameterSet::XParameterSet() : 
entries_() {

}


//---
XParameterSet::XParameterSet(const XParameterSet& other) :
entries_() {

//  BOOST_FOREACH( const EntryMap::value_type& it, orig.entries_) {
//    entries_.emplace(it.first, it.second);
//  }
  
  (*this) = other;
   
}


//---
XParameterSet::~XParameterSet() {
  
  clear();
//  BOOST_FOREACH( EntryMap::value_type& p, entries_) {
//    delete p.second.object;
//  }
}

//---
XParameterSet& XParameterSet::operator=(const XParameterSet& other) {
  
  clear();
  BOOST_FOREACH( const EntryMap::value_type& it, other.entries_) {
    entries_.emplace(it.first, it.second);
  }
  
  return (*this);
}


//---
void
XParameterSet::clear() {
  BOOST_FOREACH( EntryMap::value_type& p, entries_) {
    delete p.second.object;
  }
  entries_.clear();
}


//---
std::set<std::string>
XParameterSet::names() const {
  std::set<std::string> names;

 std::transform(entries_.begin(), entries_.end(), std::inserter(names, names.end()), boost::bind(&EntryMap::value_type::first, _1));
 return names;
}


//---
size_t XParameterSet::count(const std::string& name) const {
  return entries_.count(name);
}


//---
size_t XParameterSet::size() const {
  return entries_.size();
}



//---
std::string XParameterSet::type() const {
  return "parameterset";
}


//---
void
XParameterSet::setValue(const xdata::Serializable& s) {
  this->operator =(dynamic_cast<const XParameterSet&>(s));
}

//---
int XParameterSet::equals(const xdata::Serializable& other) {
  const XParameterSet& o = dynamic_cast<const XParameterSet&>(other);
  return this->entries_ == o.entries_;
}


//---
std::string XParameterSet::toString() throw (xdata::exception::Exception) {
  return "";
}


//---
void XParameterSet::fromString(const std::string&) throw (xdata::exception::Exception) {
}


//---
void XParameterSet::erase(const std::string& name) {
  // Pop and delete
  delete pop(name);
}


//---
xdata::Serializable&
XParameterSet::get( const std::string& name ) {
  EntryMap::iterator it = entries_.find(name);
  if ( it == entries_.end() ) {
    throw XParameterNotFound(name +" not found");
  }

  return *(it->second.object);
}


//---
const xdata::Serializable&
XParameterSet::get( const std::string& name ) const {
  return const_cast<const xdata::Serializable&>(const_cast<const XParameterSet*>(this)->get(name));
}


//---
void
XParameterSet::set(const std::string& name, const xdata::Serializable& data) {
  // Type check?
  get(name).setValue(data);
}

//---
xdata::Serializable*
XParameterSet::pop(const std::string& name) {
  EntryMap::iterator entryIt = entries_.find(name);

  if ( entryIt == entries_.end() ) {
      throw XParameterNotFound(name +" not found");
  }

  xdata::Serializable* s = entryIt->second.object;
  
  entries_.erase(entryIt);
  
  return s;
}

//---
XParameterSet::XEntry::XEntry(const std::type_info* t, XCloner c, xdata::Serializable* s) :
  typeinfo(t),
  cloner(c),
  object(s) {
}


//---
XParameterSet::XEntry::XEntry(const XEntry& orig) :
  typeinfo(orig.typeinfo),
  cloner( orig.cloner),
  object( orig.cloner(orig.object)) {

}


//---
bool 
XParameterSet::XEntry::operator==(const XEntry& other) const {

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

XParameterSet::Inserter::Inserter(XParameterSet* ps) {
    xps_ = ps;
}


} // core
} // swatch