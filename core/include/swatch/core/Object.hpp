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

namespace swatch
{
  namespace core
  {
    // Forward declaration of the Object class
    class Object;
    // Forward declaration of the Functionoid class
    class Functionoid;
  }
}


namespace swatch
{
  namespace core
  {

    // Forward declaration of the object-streaming operator
    std::ostream& operator<< ( std::ostream& aStr , const swatch::core::Object& aObject );

    /**
      Hierarchical base class for swatch classes. Inspired by uhal::Node
    */
    class Object
    {
      public:
        /**
          Class to iterate over the children of an object
        */
        class iterator : public std::iterator< std::forward_iterator_tag , Object >
        {
            friend class Object;
            typedef std::deque< std::deque< Object* >::const_iterator > Stack;
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
              @param dummy a dummy arg to distinguish between pre- and postfix operators
              @return a copy of the iterator before the increment
            */
            iterator operator++ ( int dummy );

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
            Object* begin_;
            /// A stack to store the vertical position in the hierarchy
            Stack itStack_;
        };

        friend std::ostream& ( operator<< ) ( std::ostream& aStr , const swatch::core::Object& aObject );
        friend class Functionoid;

        /**
          Constructor
          @param aId The ID string of the object
        */
        explicit Object ( const std::string& aId );

        /**
          Constructor with parameter set
          @param aId The ID string of the object
          @param params A parameter set associated with the objects
        */
        Object ( const std::string& aId, const XParameterSet& params );

        /**
          Destructor
        */
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
          Return the type info associated with the stored entry
          @return the type info associated with the stored entry       
        */
        const std::type_info& type() const;

        /**
          Return the type name associated with the stored entry
          @return the type name associated with the stored entry       
        */
        std::string typeName() const;

        /**
          Return the id of the object
          @return the id of the object       
        */
        const std::string& id() const;

        /**
          Return the dot-delimeted path through the tree to this object
          @return the dot-delimeted path through the tree to this object      
        */
        const std::string path() const;

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
          Navigate down the dot-delimeted path from the current object and return the target
          @param aId a dot-delimeted path from the current object to the target
          @return the object indicated by the dot-delimeted path or throw if the target doesn't exist
        */
        Object& getObj ( const std::string& aId ) const;

        /**
          Navigate down the dot-delimeted path from the current object and return the target, dynamic cast to type T
          @param aId a dot-delimeted path from the current object to the target
          @return the object indicated by the dot-delimeted path, dynamic cast to type T, or throw if the target doesn't exist
        */
        template<typename T>
        T* getObj ( const std::string& aId );

//         template<typename T>
//         std::deque<T*> getChildrenOfType();

        /**
          Return the parameter-set of the object
          @return the parameter-set of the object       
        */
        const XParameterSet& pset() const;

      protected:

        /**
          Add a new Object as a child of the current Object
          @warning the parent takes ownership of the child and takes responsibility for deleting it
          @param aChild a pointer to a new Object
        */
        void addObj ( Object* aChild );

        /**
          Container for child objects. Deleted by destructor
        */
        std::deque< Object* > children_;

        /**
          Map of children
        */
        boost::unordered_map< std::string, Object* > objectsChart_;

      private:
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
          @param basePath String that is prepended to object paths
          @param chart Map that will be filled with all descendants
        */
        void getCharts ( const std::string& basePath, boost::unordered_map<std::string, Object*>& chart ) const;

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

        /**
          The ID of the current Object
        */
        std::string id_;

        /**
          Pointer to the parent of the current Object
        */
        Object* parent_;

        /**
          The parameter-set of the current Object
        */
        const XParameterSet pSet_;

        friend class iterator;
        friend class ObjectView;
    };

    class ObjectView : public Object
    {
      public:

        ObjectView ( const std::string& aId );

        ObjectView ( const std::string& aId, const XParameterSet& aSet );

        virtual ~ObjectView();

      protected:

        void addObj ( Object* aChild, const std::string& aAlias );

        void addObj ( Object* aChild );

    };


    template<typename T>
    T* Object::getObj ( const std::string& aId )
    {
      return dynamic_cast<T*> ( & this->getObj ( aId ) );
    }


//     template<typename T>
//     std::deque<T*> Object::getChildrenOfType()
//     {
//       std::deque<T*> lRet;
// 
//       for ( std::deque<Object*>::const_iterator lIt = children_.begin(); lIt != children_.end(); ++lIt )
//       {
//         T* lChild ( dynamic_cast<T*> ( *lIt ) );
// 
//         if ( lChild )
//         {
//           lRet.push_back ( lChild );
//         }
//       }
// 
//       return lRet;
//     }

  } // namespace core
} // namespace swatch



#endif	/* __SWATCH_TEST_OBJECT_HPP__ */

