/**
 * @file    AbstractFactory.hxx
 * @author  Alessandro Thea
 * @brief   Implementation of the templated methods
 * @date    July 2014
 *
 */

#ifndef __SWATCH_CORE_ABSTRACTFACTORY_HXX__
#define	__SWATCH_CORE_ABSTRACTFACTORY_HXX__

#include "xdata/String.h"
namespace swatch {
namespace core {

template<typename T>
AbstractFactory<T>* AbstractFactory<T>::me_ = 0x0;

template<typename T>
AbstractFactory<T>* AbstractFactory<T>::get() {
    if ( !me_ ) 
        me_ = new AbstractFactory<T>();
    
    return me_;
}

template<typename T>
T* AbstractFactory<T>::make( const std::string& aCreatorId, const std::string& aId, const swatch::core::XParameterSet& aPars ) {
    
    typename boost::unordered_map< std::string , boost::shared_ptr<CreatorInterface> >::const_iterator lIt = creators_.find ( aCreatorId );

    if ( lIt == creators_.end() ) {
        throw std::runtime_error("Class "+aCreatorId+" not found");
    }
    
    return (*lIt->second)( aId, aPars );
}

template<typename T>
T* AbstractFactory<T>::make(const core::XParameterSet& aPars) {
   
    if ( !aPars.has("class") ) {
        throw std::runtime_error("'class' parameter not found in parameter set");
    }
   
    if ( !aPars.has("name") ) {
        throw std::runtime_error("'name' parameter not found in parameter set");
    }
   
    return this->make( aPars.get<xdata::String>("class"), aPars.get<xdata::String>("name"), aPars);
}

template<typename T>
template<typename U>
T* AbstractFactory<T>::BasicCreator<U>::operator ()(const std::string& aId, const swatch::core::XParameterSet& aPars) {
    return new U( aId, aPars );
}

template <typename T>
template <typename K>
bool AbstractFactory<T>::add(const std::string& aName) {
    typename boost::unordered_map<std::string, boost::shared_ptr<CreatorInterface> >::const_iterator lIt = creators_.find(aName);

    if (lIt != creators_.end()) {
        return false;
    }

    creators_[aName] = boost::shared_ptr<CreatorInterface> (new K());
    return true;
}

} // namespace test
} // namespace swatch


#endif	/* __SWATCH_CORE_ABSTRACTFACTORY_HXX__ */

