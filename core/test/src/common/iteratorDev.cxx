#include <string>
#include <stdint.h>
#include <iterator>


#include "boost/unordered_map.hpp"
// #boost_

// template<typename Iterator>
// class _iterator_base : public Iterator {
// private:
//     _iterator_base( M::iterator it );

// };

typedef boost::unordered_map<std::string,uint32_t> MyMap;


struct KeyValue {
    std::string key;
    uint32_t value;
};

template<typename T, typename I>
class _iterator_base : public std::iterator< std::forward_iterator_tag , KeyValue > {
public:
    _iterator_base() : iter_() {}

    _iterator_base( T* map ) {
        iter_ = map->begin();
        update();
    }

    _iterator_base( const _iterator_base& other ) {
        iter_ = other.iter_;
        update();
    }

    KeyValue& value() const { return cache_; };
    KeyValue& operator*() const { return value(); }
    KeyValue* operator->() const { return &value();}

    _iterator_base operator++() { iter_++; update(); return *this; }
    _iterator_base operator++( int ) { 
        _iterator_base orig = *this;
        ++(*this);  // do the actual increment
        return orig;
    }

    bool operator== ( const _iterator_base& it ) const { return iter_ == it.iter_; }
    bool operator!= ( const _iterator_base& it ) const { return iter_ != it.iter_; }
private:

    void update() {

      if ( iter_.operator ->() != 0x0 ) {
        cache_.key = iter_->first;
        cache_.value = iter_->second;
      } else {

      }
    }
    
    I iter_;
    mutable KeyValue cache_;
};


int main(int argc, char const *argv[])
{

    
    MyMap mymap;
    mymap["a"] = 7;
    mymap["b"] = 11; 

    MyMap::iterator i = mymap.begin();
    
    
    

  
    _iterator_base<MyMap, MyMap::iterator> j(&mymap);
    
    std::cout << " k:" <<  j->key << " v:" << j->value << std::endl;

    ++j;
    
    std::cout << " k:" <<  j->key << " v:" << j->value << std::endl;

    ++j;

    std::cout << "is end: " << (j == _iterator_base<MyMap, MyMap::iterator>()) << std::endl;
    
    
    _iterator_base<const MyMap, MyMap::const_iterator> k(&mymap);
    
    std::cout << " k:" <<  k->key << " v:" << k->value << std::endl;

    ++k;
    
    std::cout << " k:" <<  k->key << " v:" << k->value << std::endl;

    ++k;

    std::cout << "is end: " << (k == _iterator_base<const MyMap, MyMap::const_iterator>()) << std::endl;
    /* code */
    return 0;
}