/* 
 * File:   ParameterSet.hxx
 * Author: ale
 *
 * Created on August 1, 2014, 12:30 PM
 */

#ifndef __swatch_core_parameterset_hxx__
#define	__swatch_core_parameterset_hxx__

#include "swatch/core/exception.hpp"

//#include <boost/static_assert.hpp>
#include <boost/mpl/assert.hpp>

namespace swatch {
namespace core {

template<typename T>
void ParameterSet::set(const std::string& aKey, const T& aValue) {
    using namespace std;
    // no pointers allowed beyond this point
//    BOOST_STATIC_ASSERT( ! boost::is_pointer<T>::value ); 
    BOOST_MPL_ASSERT_MSG(
            ! boost::is_pointer<T>::value,
            POINTERS_ARE_NOT_ALLOWED_IN_PSET,
            (T)
            ); 
//    cout << "---" << demangleName( typeid(aValue).name() ) << endl;
//    cout << aKey << " is_pod " << boost::is_pod<T>::value << endl;
//    cout << aKey << " is_pointer " << boost::is_pointer<T>::value << endl;
//    cout << aKey << " is_reference " << boost::is_reference<T>::value << endl;
    this->operator[](aKey) = aValue;
}


template<typename T>
T&
ParameterSet::get(const std::string& aKey) {
    iterator it = this->find(aKey);
    //TODO runtime_error -> dedicated error
    if (it == this->end()) throw std::runtime_error("Parameter not found:" + aKey);
    if ( typeid(T) != it->second.type() ) {
        std::stringstream exc;
        exc << "Cannot cast " << demangleName(it->second.type().name()) << " to " << demangleName(typeid(T).name());
        throw ParameterNotFound(exc.str());
    }
    
    T* ptr = boost::any_cast<T>(&(it->second));
    return *ptr;
}


template<typename T>
const T&
ParameterSet::get(const std::string& aKey) const {
    const_iterator it = this->find(aKey);
    //TODO runtime_error -> dedicated error
    if (it == this->end()) throw std::runtime_error("Parameter not found:" + aKey);
    if ( typeid(T) != it->second.type() ) {
        std::stringstream exc;
        exc << "Cannot cast " << demangleName(it->second.type().name()) << " to " << demangleName(typeid(T).name());
        throw ParameterNotFound(exc.str());
    }
    
    const T* const_ptr = boost::any_cast<T>(&(it->second));
    return *const_ptr;
}


template<typename T>
const T&
ParameterSet::get(const std::string& aKey, const T& aDefault ) const {
    const_iterator it = this->find(aKey);

    if (it == this->end()) return aDefault;
    if ( typeid(T) != it->second.type() ) {
        std::cout << "Cannot cast " << demangleName(it->second.type().name()) << " to " << demangleName(typeid(T).name()) << std::endl;
    }

    const T* const_ptr = boost::any_cast<T>(&(it->second));    
    return *const_ptr;
}


template<typename T>
ParameterSet::Inserter& ParameterSet::Inserter::operator()(const std::string& aKey, const T& aValue) {
    mPS->set(aKey, aValue);
    return *this;
}


template<typename T>
ParameterSet::Inserter ParameterSet::insert(const std::string& aKey, const T& aValue) {
    this->set(aKey, aValue);
    return Inserter(this);
}

} // namespace core
} // namespace swatch


#endif	/* __swatch_core_parameterset_hxx__ */

