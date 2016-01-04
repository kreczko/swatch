/**
 * @file    ReadWriteXParameterSet.hpp
 * @author  Tom Williams, Alessandro Thea
 * @date    June 2015
 *
 */

#ifndef __SWATCH_CORE_READWRITEXPARAMETERSET_HPP__
#define __SWATCH_CORE_READWRITEXPARAMETERSET_HPP__

// C++ headers
#include <typeinfo>

// Boost headers
#include "boost/unordered_map.hpp"

// SWATCH headers
#include "swatch/core/exception.hpp"
#include "swatch/core/XParameterSet.hpp"



namespace swatch {
namespace core {

class ReadOnlyXParameterSet;
    
    
//! Generic container for xdata::Serializable objects, with shared ownership via boost::shared_ptr<xdata::Serializable>, and read-write access to the underlying data
class ReadWriteXParameterSet : public XParameterSet {
public:

  ReadWriteXParameterSet();
  
  virtual ~ReadWriteXParameterSet();


  /* --- DATA ACCESS --- */
  
  bool operator==( const ReadWriteXParameterSet& ) const;
  
  /**
   * Size of the collection
   * @return Number of stored parameters
   */
  size_t size() const;

  /**
   * List of names of stored parameters.
   * @return names of stored parameters
   */
  std::set<std::string> keys() const;

  //! Returns whether parameter with given name exists
  bool has( const std::string& aName ) const;

  xdata::Serializable& get( const std::string& aName );

  const xdata::Serializable& get( const std::string& aName ) const;

  xdata::Serializable& operator[]( const std::string& aName );

  const xdata::Serializable& operator[]( const std::string& aName ) const;

  template<typename T>
  const T& get( const std::string& aName ) const;

  virtual std::string parameterAsString(const std::string& aName) const;


  /* --- ADDING ENTRIES TO SET --- */
  
  /**
   * Adopt a parameter in the set; the data that is pointed to is not copied, and stored internally via a shared_ptr. The data type must be a derived from xdata::Serializable
   * 
   * @param aName Parameter name
   * @param aData Pointer to import in the set
   */
  template<typename T>
  void adopt( const std::string& aName , const boost::shared_ptr<T>& aData );

  /**
   * Add a parameter to the set, by copying data into the set
   * 
   * @param aName Parameter name
   * @param aData Value to copy in the set.
   */
  template<typename T>
  void add( const std::string& aName , const T& aData );

  void deepCopyFrom(const ReadWriteXParameterSet& aOtherSet);
  
  void erase( const std::string& aName );


private:

  template<typename T>
  static xdata::Serializable* clone( const xdata::Serializable* aOther );

  typedef xdata::Serializable* (*XCloner_t)( const xdata::Serializable* );

  struct XEntry {

    /**
     * Standard constructor
     *  
     * @param aType Type info pointer
     * @param aCloner Cloner function pointer
     * @param aData Data to store in the entry
     */
    XEntry( const std::type_info* aType, XCloner_t aCloner, const boost::shared_ptr<xdata::Serializable>& aData );
    
    /**
     * Copy constructor
     * The object pointer is not copied but a new instance is created instead.
     * 
     * @param orig Entry to copy from
     */
    XEntry( const XEntry& orig );

    bool operator==(const XEntry &other) const;
    
    const std::type_info* typeinfo;

    xdata::Serializable* (*cloner)( const xdata::Serializable* );

    boost::shared_ptr<xdata::Serializable> object;
  };
  
  typedef boost::unordered_map<std::string, XEntry> EntryMap_t;
  
  std::pair<EntryMap_t::iterator,bool> emplace( const std::string& name, const std::type_info* t, XCloner_t c, const boost::shared_ptr<xdata::Serializable>& s );
  std::pair<EntryMap_t::iterator,bool> emplace( const std::string& name, const XEntry& );

  //! Map storing the values
  EntryMap_t mEntries;
  
  friend class ReadOnlyXParameterSet;
  
  ReadWriteXParameterSet(const ReadWriteXParameterSet& orig); // non scopyable
  ReadWriteXParameterSet& operator=( const ReadWriteXParameterSet& ); // non copyable
};


} // namespace core
} // namespace swatch

#include "swatch/core/ReadWriteXParameterSet.hxx"

#endif /* __SWATCH_CORE_READWRITEXPARAMETERSET_HPP__ */
