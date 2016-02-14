/**
 * @file    Object.hpp
 * @author  Alessandro Thea
 * @brief   Base class to exercise the Object interface
 * @date    August 2014
 *
 */

#ifndef __SWATCH_CORE_OBJECT_HPP__
#define __SWATCH_CORE_OBJECT_HPP__


// Standard headers
#include <typeinfo>
#include <iosfwd>

// boost headers
#include "boost/noncopyable.hpp"
#include "boost/type_traits/is_base_of.hpp"
#include "boost/unordered_map.hpp"

// SWATCH headers
#include "swatch/core/exception.hpp"


namespace swatch {
namespace core {

// Forward declarations      
class Object;
class ObjectView;

// Forward declaration of the object-streaming operator
std::ostream& operator<< ( std::ostream& aStr , const swatch::core::Object& aObject );

/**
  Hierarchical base class for swatch classes. Inspired by uhal::Node
*/
//! Base class allowing for registration of children using string ID; child objects must be heap-allocated are owned by their parent
class Object : public boost::noncopyable
{
  public:
  //! Functor interface used for destruction of objects in object tree (allows deletion to be delayed until all threads are done with the object)
  class Deleter
  {
  public:
    Deleter() {}
    virtual ~Deleter() {}

    virtual void operator()(Object* aObject) = 0;
  };

          
  //! Class to iterate over the children of an object
  class iterator : public std::iterator< std::forward_iterator_tag , Object >
  {
    typedef std::deque< std::deque< std::pair<Object*,Deleter*> >::const_iterator > Stack_t;
  public:
    /**
      Default constructor
    */
    iterator();

    /**
      Constructor from pointer to Object
      @param aBegin Top-level Object over which to iterate
    */
    iterator ( Object* aBegin );

    /**
      Copy constructor
      @param aOrig An Object-iterator to copy
    */
    iterator ( const iterator& aOrig );

    /**
      Destructor
    */
    virtual ~iterator();

    /**
      Dereferencing function
      @return the underlying object
    */
    Object& value() const;

    /**
      Dereferencing function
      @return the underlying object
    */
    Object& operator*() const;

    /**
      Dereferencing function
      @return pointer to the underlying object
    */
    Object* operator->() const;

    /**
      Move the iterator to point to the next object
      @return whether the new iterator is valid 
    */
    bool next();

    /**
      Prefix operator - increment the iterator
      @return new value of the iterator
    */
    iterator& operator++();

    /**
      Postfix operator - increment the iterator
      @param aDummy a dummy arg to distinguish between pre- and postfix operators
      @return a copy of the iterator before the increment
    */
    iterator operator++ ( int aDummy );

    /**
      Equality operator
      @param aIt an operator to compare whether two iterators are equal
      @return whether the two iterators are equal
    */
    bool operator== ( const iterator& aIt ) const;

    /**
      Inequality operator
      @param aIt an operator to compare whether two iterators are different
      @return whether the two iterators are different
    */
    bool operator!= ( const iterator& aIt ) const;

  private:
    /// Pointer to the current object or NULL if there are no more objects available
    Object* mBegin;
    /// A stack to store the vertical position in the hierarchy
    Stack_t mItStack;
  };

        
  friend std::ostream& ( operator<< ) ( std::ostream& aStr , const swatch::core::Object& aObject );

  /**
    Constructor
    @param aId The ID string of the object
  */
  explicit Object ( const std::string& aId );


  //! Destructor
  virtual ~Object();

  /**
    Construct an iterator starting at the current Object
  */
  iterator begin();

  /**
    Return a null iterator which will return true if we are comparing against the entry beyond the last valid object 
  */
  iterator end();

  /**
    Return the type name associated with the stored entry
    @return the type name associated with the stored entry       
  */
  std::string getTypeName() const;

  /**
    Return this object's ID 
    @return this object's ID string       
  */
  const std::string& getId() const;

  /**
    Return the dot-delimited path through the tree to this object
    @return the dot-delimited path through the tree to this object      
  */
  std::string getPath() const;

  /**
    Return the full path to each of the children of the current object 
    @return the full path to each of the children of the current object 
  */
  std::vector<std::string> getPaths() const;

    /**
      Return ID strings of all children of this object
      @return ID strings of all children of this object
    */
  std::vector<std::string> getChildren() const;

  /**
    Return relative ID path of all this descendents (children, grandchildren, etc) of this object
    @return relative ID path of all this descendents (children, grandchildren, etc) of this object
  */
  std::vector<std::string> getDescendants() const;

  /**
    Navigate down the dot-delimited path from the current object and return the target
    @param aId a dot-delimited path from the current object to the target
    @throw ObjectDoesNotExist if object of specified ID path doesn't exist
    @return the object indicated by the dot-delimited path, or throw if the target doesn't exist
  */
  Object& getObj ( const std::string& aId );

  /**
    Navigate down the dot-delimited path from the current object and return the target
    @param aId a dot-delimited path from the current object to the target
    @throw ObjectDoesNotExist if object of specified ID path doesn't exist
    @return the object indicated by the dot-delimited path, or throw if the target doesn't exist
  */
  const Object& getObj ( const std::string& aId ) const;

  /**
    Navigate down the dot-delimited path from the current object and return the target, dynamic cast to type T
    @param aId a dot-delimited path from the current object to the target
    @throw ObjectDoesNotExist if object of specified ID path doesn't exist
    @return the object indicated by the dot-delimited path, dynamic casted to type T; NULL pointer if dynamic cast fails; or throw if the target doesn't exist
  */
  template<typename T>
  T* getObjPtr ( const std::string& aId );

  /**
    Navigate down the dot-delimited path from the current object and return the target, dynamic cast to type T
    @param aId a dot-delimited path from the current object to the target
    @throw ObjectDoesNotExist if object of specified ID path doesn't exist
    @return the object indicated by the dot-delimited path, dynamic casted to type T; NULL pointer if dynamic cast fails; or throw if the target doesn't exist
  */
  template<typename T>
  const T* getObjPtr ( const std::string& aId ) const;

//         template<typename T>
//         std::deque<T*> getChildrenOfType();

protected:

  /**
    Add a new Object as a child of the current Object; throws if this object already has another child object with the same ID
    @warning the parent takes ownership of the child and takes responsibility for deleting it
    @param aChild a pointer to a new Object
  */
  void addObj ( Object* aChild );

  /**
    Add a new object as a child of the current Object
    @warning the parent takes ownership of the child, and takes responsibility for deleting it using the supplied functor
    @param aChild a pointer to a new Object
    @param aDeleter a functor that inherits from Object::Deleter. Must be copy-constructible. The functor's void operator()(Object*) method will be called later in order to delete this object
  */
  template <class T>
  void addObj ( Object* aChild, T aDeleter);

private:
  /**
    Return the ancestor (parent, grandparent, ...) of the object at specified depth
    @param aDepth the depth of the target to retrieve (0=current, 1=parent, 2=grandparent, ...)
    @return the target object or NULL if no such target exists
  */
  const Object* getAncestor ( const uint32_t& aDepth = 1 ) const;

  /**
    Return the ancestor (parent, grandparent, ...) of the object at specified depth
    @param aDepth the depth of the target to retrieve (0=current, 1=parent, 2=grandparent, ...)
    @return the target object or NULL if no such target exists
  */
  Object* getAncestor ( const uint32_t& aDepth = 1 );

  /**
    Recursive stream-formatting function
    @param aStr a stream to append the formatted output to
    @param aIndent the current layer of recursion = the amount of indentation
  */
  void print ( std::ostream& aStr , const uint32_t& aIndent = 0 ) const;

  /**
    Fills up a map with all descendants of this object; the path of each object in the map is prepended by the string specified by the caller
    @param aBasePath String that is prepended to object paths
    @param aChart Map that will be filled with all descendants
  */
  void getCharts ( const std::string& aBasePath, boost::unordered_map<std::string, Object*>& aChart ) const;

  /**
    Fill a deque with the ancestors (parent, grandparent, ...) of the current object
    @param aGenealogy a deque to be filled with the ancestors (parent, grandparent, ...) of the current object
  */
  void getAncestors ( std::deque< const Object* >& aGenealogy ) const;

  /**
    Set the owner of the current Object
    @param aParent an Object to be made the parent of the current Object
  */
  void setParent ( Object* aParent );

  //! This object's ID
  std::string mId;

  //! Pointer to this object's parent
  Object* mParent;

  //! Container for child objects, and their deleter functors. Children in this list are deleted by the destructor
  std::deque< std::pair< Object*, Deleter* > > mChildren;

  //! Map of children
  boost::unordered_map< std::string, Object* > mObjectsChart;
  
  friend class ObjectView;
};


class ObjectView : public Object
{
public:

  ObjectView ( const std::string& aId );
    
  virtual ~ObjectView();

protected:

  void addObj ( Object* aChild, const std::string& aAlias );

  void addObj ( Object* aChild );
};


template<typename T>
T* Object::getObjPtr ( const std::string& aId )
{
  return dynamic_cast<T*> ( & this->getObj ( aId ) );
}


template<typename T>
const T* Object::getObjPtr ( const std::string& aId ) const
{
  return dynamic_cast<const T*> ( & this->getObj ( aId ) );
}


template<class T>
void Object::addObj (Object* aChild, T aDeleter)
{
  BOOST_STATIC_ASSERT_MSG( (boost::is_base_of<Object::Deleter, T >::value) , "class T must be a descendant of swatch::core::Object::Deleter" );

  aChild->setParent(this);

  // Insure the child does not have a twin
  if (mObjectsChart.find(aChild->getId()) != mObjectsChart.end()) {
    throw std::runtime_error(aChild->getId() + " already exists in this family");
  }

  mChildren.push_back(std::make_pair(aChild, new T(aDeleter)));
  mObjectsChart.insert(std::make_pair(aChild->getId(), aChild));
}


DEFINE_SWATCH_EXCEPTION(InvalidObjectId);
DEFINE_SWATCH_EXCEPTION(ObjectDoesNotExist);


} // namespace core
} // namespace swatch


#endif	/* __SWATCH_CORE_OBJECT_HPP__ */

