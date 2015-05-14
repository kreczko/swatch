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
            iterator ( iterator& aOrig );
  
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

    explicit Object( const std::string& aId );

    Object( const std::string& aId, const XParameterSet& params );
    
    virtual ~Object();
    
    iterator begin();
    iterator end();
    
    const std::type_info& type();
    std::string typeName();
    
    const std::string& id() const; 
    const std::string path() const;

    std::vector<std::string> getPaths() const;

    std::vector<std::string> getChildren();

    Object* getObj(const std::string& aId);
    
    template<typename T>
    T* getObj(const std::string& aId);
    
    const XParameterSet& pset() { return pSet_; }
    
protected:

    void addObj( Object* aChild );
    
    //! Container for child objects. Deleted by destructor
    std::deque< Object* > children_;
    
    //! Map of children and their child nodes
    boost::unordered_map< std::string, Object* > objectsChart_;

    Object* getParent();

private:
    void print( std::ostream& aStr , const uint32_t& aIndent = 0 ) const;
    
    void getCharts( std::string path, boost::unordered_map<std::string, Object*>& ) const;
    
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


} // namespace core
} // namespace swatch



#endif	/* __SWATCH_TEST_OBJECT_HPP__ */

