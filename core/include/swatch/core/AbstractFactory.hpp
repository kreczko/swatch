/* 
 * File:   AbstractFactory.hpp
 * Author: ale
 *
 * Created on July 31, 2014, 4:50 PM
 */

#ifndef SWATCH_CORE_ABSTRACTFACTORY_HPP
#define	SWATCH_CORE_ABSTRACTFACTORY_HPP

// C++ Headers
#include <string>
#include "swatch/core/ParameterSet.hpp"

// Boost Headers
#include <boost/noncopyable.hpp>
#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>

namespace swatch {
namespace core {

template<typename T>
class AbstractFactory : public boost::noncopyable {
private:
    AbstractFactory() {}

public:
    template< typename A, typename U > friend struct ClassRegistrationHelper;
    template< typename A, typename K > friend struct CreatorRegistrationHelper;
    
    typedef T Product;
    
    virtual ~AbstractFactory() {}
    
    static AbstractFactory* get();

    T* make( const std::string& aCreatorId, const std::string& aId, const swatch::core::ParameterSet& params );

    T* make( const swatch::core::ParameterSet& params );

    class CreatorInterface {
    public:
        virtual T* operator()( const std::string& aId, const swatch::core::ParameterSet& params ) = 0;
    };
    

private:
    template<typename U>
    class BasicCreator : public CreatorInterface {
    public:
        virtual T* operator() ( const std::string& aId, const swatch::core::ParameterSet& params );
    };
    
    template<typename K>
    bool add(const std::string& aCreatorName);
    
    private:
        
    static AbstractFactory* me_;
    boost::unordered_map<std::string, boost::shared_ptr<CreatorInterface> > creators_;

};

template< typename A, typename U >
struct ClassRegistrationHelper {
    ClassRegistrationHelper(const std::string& aClassName) {
//        AbstractFactory<T>::get()->add< AbstractFactory<T>::BasicCreator<U> >(aClassName);
        AbstractFactory<A>::get()->template add< typename AbstractFactory<A>::template BasicCreator<U> > (aClassName );
    }
};

template< typename T, typename K >
struct CreatorRegistrationHelper {
    CreatorRegistrationHelper(const std::string& aCreatorName) {
        AbstractFactory<T>::get()->template add< K >(aCreatorName);
    }
};

} // namespace core
} // namespace swatch

#include "AbstractFactory.hxx"


#endif	/* SWATCH_CORE_ABSTRACTFACTORY_HPP */

