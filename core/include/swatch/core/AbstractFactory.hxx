/* 
 * File:   AbstractFactory.hxx
 * Author: ale
 *
 * Created on July 31, 2014, 4:51 PM
 */

#ifndef SWATCH_CORE_ABSTRACTFACTORY_HXX
#define	SWATCH_CORE_ABSTRACTFACTORY_HXX

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
T* AbstractFactory<T>::make( const std::string& aCreatorId, const std::string& aId, const swatch::core::ParameterSet& params ) {
    
    typename boost::unordered_map< std::string , boost::shared_ptr<CreatorInterface> >::const_iterator lIt = creators_.find ( aCreatorId );

    if ( lIt == creators_.end() ) {
        throw std::runtime_error("Class "+aCreatorId+" not found");
    }
    
    return (*lIt->second)( aId, params );
}

template<typename T>
T* AbstractFactory<T>::make(const core::ParameterSet& params) {
   
    if ( !params.has("class") ) {
        throw std::runtime_error("'class' parameter not found in parameter set");
    }
   
    if ( !params.has("name") ) {
        throw std::runtime_error("'name' parameter not found in parameter set");
    }
   
    return this->make( params.get<std::string>("class"), params.get<std::string>("name"), params);
}

template<typename T>
template<typename U>
T* AbstractFactory<T>::BasicCreator<U>::operator ()(const std::string& aId, const swatch::core::ParameterSet& params) {
    return new U( aId, params );
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


#endif	/* SWATCH_CORE_ABSTRACTFACTORY_HXX */

