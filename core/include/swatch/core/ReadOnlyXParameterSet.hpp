/**
 * @file    ReadOnlyXParameterSet.hpp
 * @author  Tom Williams, Alessandro Thea
 * @date    June 2015
 *
 */

#ifndef __SWATCH_CORE_READONLYXPARAMETERSET_HPP__
#define __SWATCH_CORE_READONLYXPARAMETERSET_HPP__


// boost headers
#include <boost/unordered_map.hpp>

// SWATCH headers
#include "swatch/core/XParameterSet.hpp"



namespace swatch {
namespace core {

// Forward declaration
class ReadWriteXParameterSet;    

//! Generic container for xdata::Serializable objects, with shared ownership via boost::shared_ptr<xdata::Serializable>, and read-only access to the underlying data
class ReadOnlyXParameterSet : public XParameterSet {
public:

  ReadOnlyXParameterSet();

  //! Copy constructor; performs SHALLOW COPY
  ReadOnlyXParameterSet(const ReadOnlyXParameterSet& orig);

  //! Constructor from any XParameterSet; performs SHALLOW COPY
  ReadOnlyXParameterSet(const XParameterSet& orig);

  virtual ~ReadOnlyXParameterSet();

  
  bool operator==(const ReadOnlyXParameterSet& o) const;
  
  
  /**
   * Size of the collection
   * @return Number of stored parameters
   */
  virtual size_t size() const;

  /**
   * List of names of stored parameters.
   * @return names of stored parameters
   */
  virtual std::set<std::string> keys() const;

  //! Returns whether parameter with given name exists
  virtual bool has( const std::string& name ) const;

  virtual const xdata::Serializable& get( const std::string& name ) const;

  virtual const xdata::Serializable& operator[]( const std::string& name ) const;

  template<typename T>
  const T& get( const std::string& name ) const { return this->XParameterSet::get<T>(name); }

  virtual std::string parameterAsString(const std::string& name) const;
  
  /**
   * Adopt a parameter in the set; the data that is pointed to is not copied, and stored internally via a shared_ptr. The data type must be a derived from xdata::Serializable
   * 
   * @param name Parameter name
   * @param data Pointer to import in the set
   */
  void adopt( const std::string& name , const boost::shared_ptr<xdata::Serializable>& data );
  
  /**
   * Adopt a parameter that's already stored in another the set; the data that is pointed to is not copied - instead its shared between the parameter sets.
   * 
   * @param name Parameter name
   */
  void adopt( const std::string& name , const ReadOnlyXParameterSet& set);

  /**
   * Adopt a parameter that's already stored in another the set; the data that is pointed to is not copied - instead its shared between the parameter sets.
   * 
   * @param name Parameter name
   */
  void adopt( const std::string& name , const ReadWriteXParameterSet& set);


  virtual void erase( const std::string& name );


private:
  typedef boost::unordered_map<std::string, boost::shared_ptr<xdata::Serializable> > EntryMap;

  EntryMap entries_;
};


} // namespace core
} // namespace swatch

#endif /* __SWATCH_CORE_READONLYXPARAMETERSET_HPP__ */
