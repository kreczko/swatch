/**
 * @file   iteratorDev.cxx
 * @author Alessandro Thea
 * @brief  Development bench ofr XParameterSet iterator
 * @date   February 2015  
 */

#include <string>
#include <stdint.h>
#include <iterator>

#include "boost/unordered_map.hpp"

/**
 * Attempt to create an iterator pattern wrapped around unordered_map's iterators.
 * Ultimately this is meant to become an iterator for XParameterSet.
 * The pattern came out a little complicated... so best leaving it here.
 * By the way, this horrible hack is required because xdata::Serializable
 * doesn't have a "clone" method and, therefore, XParameterSet needs a cloner method.
 */

typedef boost::unordered_map<std::string,uint32_t> MyMap;

template<typename I, typename V>
class _iterator_base;

template<typename I>
struct KeyValue {

  KeyValue() : iter_() {}

  typename I::value_type::first_type & key() const { return iter_->first; }
  typename  I::value_type::second_type & value() { return iter_->second; }
  const typename I::value_type::second_type & value() const { return iter_->second; }

private:
  KeyValue( const I& iter ) : iter_(iter) {}

  I iter_;

  template<typename T, typename V> friend class _iterator_base;
};

template<typename I, typename V>
class _iterator_base : public std::iterator< std::forward_iterator_tag ,V > {
public:

    _iterator_base() : proxy_() {}

    _iterator_base( const I& iter ) : proxy_(iter) {}

    _iterator_base( const _iterator_base& other ) {
        proxy_.iter_ = other.proxy_.iter_;
    }
    
    V* ptr() { 
      return ( proxy_.iter_.operator->() != 0 ? &proxy_ : 0x0 );
    };
    
    V& operator*() { return *ptr(); }
    V* operator->() { return ptr(); }

    _iterator_base operator++() {
      proxy_.iter_++; 
      return *this;
    }
    _iterator_base operator++( int ) { 
        _iterator_base orig = *this;
        ++(*this);  // do the actual increment
        return orig;
    }

    bool operator== ( const _iterator_base& it ) const { return proxy_.iter_ == it.proxy_.iter_; }
    bool operator!= ( const _iterator_base& it ) const { return proxy_.iter_ != it.proxy_.iter_; }
private:

    mutable V proxy_;
};

typedef _iterator_base<MyMap::iterator, KeyValue<MyMap::iterator> > my_iterator;
typedef _iterator_base<MyMap::const_iterator, const KeyValue<MyMap::const_iterator> > my_const_iterator;
int main(int argc, char const *argv[])
{

    
    MyMap mymap;
    mymap["a"] = 7;
    mymap["b"] = 11; 

    MyMap::iterator i = mymap.begin();
    
    i->second = 15;
  
    my_iterator j(mymap.begin());
    
    std::cout << " k:" <<  j->key() << " v:" << j->value() << std::endl;

    ++j;
    
    std::cout << " k:" <<  j->key() << " v:" << j->value() << std::endl;
    j->value() = 11;
    
    ++j;

   std::cout << "is end: " << (j == my_iterator()) << std::endl;
   
    MyMap::const_iterator ci = (mymap.begin());
    my_const_iterator k(ci);
   
    std::cout << " k:" << k->key() << " v:" << k->value() << std::endl;

    ++k;
    
    std::cout << " k:" <<  k->key() << " v:" << k->value() << std::endl;

    ++k;

    std::cout << "is end: " << (k == my_const_iterator() ) << std::endl;
    /* code */
    return 0;
}