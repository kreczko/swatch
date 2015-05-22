/**
 * @file    Object.hpp
 * @author  Alessandro Thea
 * @brief   Base class to exercise the Object interface
 * @date    August 2014
 *
 */

#ifndef __SWATCH_TEST_OBJECT_HPP__
#define	__SWATCH_TEST_OBJECT_HPP__

// Standard Headers
#include <typeinfo>
#include <ostream>

// Boost Headers
#include "boost/unordered_map.hpp"
#include "boost/lexical_cast.hpp"

#include "swatch/core/XParameterSet.hpp"

namespace swatch {
namespace core {
class Object;
class Functionoid;
}
}

std::ostream& operator<< ( std::ostream& aStr , const swatch::core::Object& aObject );

namespace swatch {
namespace core {

//! Base class for swatch classes. Inspired by uhal::Node
class Object {
public:
    //! Class to iterate on childrens 
    class iterator : public std::iterator< std::forward_iterator_tag , Object > {
        friend class Object;
        typedef std::deque< std::deque< Object* >::const_iterator > Stack;
        public:
            iterator();
            virtual ~iterator();
            iterator ( Object* aBegin );
            iterator ( const iterator& aOrig );
  
            Object& value() const;
            Object& operator*() const;
            Object* operator->() const;
  
            bool next();
            iterator& operator++();
            iterator operator++ ( int );
  
            bool operator== ( const iterator& aIt ) const;
            bool operator!= ( const iterator& aIt ) const;
        private:
            Object* begin_;
            Stack itStack_;
    };
    
    friend std::ostream& (::operator<<) ( std::ostream& aStr , const swatch::core::Object& aObject );
    friend class Functionoid;

    explicit Object( const std::string& aId );

    Object( const std::string& aId, const XParameterSet& params );
    
    virtual ~Object();
    
    iterator begin();
    iterator end();
    
    const std::type_info& type() const;
    std::string typeName() const;
    
    const std::string& id() const; 
    const std::string path() const;

    std::vector<std::string> getPaths() const;

    //! Returns ID string of all children of this object
    std::vector<std::string> getChildren() const;

    //! Returns relative ID path of all this descendents (children, grandchildren, etc) of this object
    std::vector<std::string> getDescendants() const;
    
    Object* getObj(const std::string& aId) const;
    
    template<typename T>
    T* getObj(const std::string& aId);

    template<typename T>
    std::deque<T*> getChildrenOfType();
    
    const XParameterSet& pset() const;
    
protected:

    void addObj( Object* aChild );
    
    //! Container for child objects. Deleted by destructor
    std::deque< Object* > children_;
    
    //! Map of children and their child nodes
    boost::unordered_map< std::string, Object* > objectsChart_;

private:
    Object* getAncestor( const uint32_t& aDepth = 1 );

    void print( std::ostream& aStr , const uint32_t& aIndent = 0 ) const;
    
    /*! Fills up a map with all descendants of this object; the path of each object in the map is prepended by the string specified by the caller
     * @param basePath String that is prepended to object paths
     * @param chart Map that will be filled with all descendants
     */
    void getCharts(const std::string& basePath, boost::unordered_map<std::string, Object*>& chart) const;
    
    void getAncestors ( std::deque< const Object* >& aGenealogy ) const;

    void setParent( Object* aParent );    
    
    std::string id_;
    
    Object* parent_;
    
    const XParameterSet pSet_;

    friend class iterator;
    friend class ObjectView;
};

class ObjectView : public Object {
public:
    
    /**
     */
    ObjectView( const std::string& aId );
    
    /**
     * Constructor 
     * @param aId Identifier of this object
     * @param aAttributes 
     */
    ObjectView( const std::string& aId, const XParameterSet& aSet );
    /**
     * @brief Destructor
     */
    virtual ~ObjectView();
    
protected:

    /**
     * 
     * @param aChild
     * @param alias
     */
    void addObj( Object* aChild, const std::string& aAlias );
    
    /**
     * 
     * @param aChild
     */
    void addObj( Object* aChild );
    
};


template<typename T>
T* Object::getObj(const std::string& aId) {
    return dynamic_cast<T*>( this->getObj(aId) );
}


template<typename T>
std::deque<T*> Object::getChildrenOfType() {
  std::deque<T*> lRet;
  for ( std::deque<Object*>::const_iterator lIt = children_.begin(); lIt != children_.end(); ++lIt ) {
    T* lChild( dynamic_cast<T*>( *lIt ) );
    if( lChild ) lRet.push_back( lChild );
  }
  return lRet;
}

} // namespace core
} // namespace swatch



#endif	/* __SWATCH_TEST_OBJECT_HPP__ */

