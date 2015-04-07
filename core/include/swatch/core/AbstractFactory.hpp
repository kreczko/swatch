/**
 * @file    AbstractFactory.hpp
 * @author  Alessandro Thea
 * @brief   Template class to provide support for abstract classes
 * @date    July 2014
 *
 */

#ifndef __SWATCH_CORE_ABSTRACTFACTORY_HPP__
#define	__SWATCH_CORE_ABSTRACTFACTORY_HPP__

// C++ Headers
#include <string>
#include "swatch/core/XParameterSet.hpp"

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
    template< typename A, typename D > friend struct ClassRegistrationHelper;
    template< typename A, typename K > friend struct CreatorRegistrationHelper;
    template< typename A, typename D > friend struct ClassRegistrationHelper2g;
    template< typename A, typename K > friend struct CreatorRegistrationHelper2g;
    
    typedef T Product;
    
    virtual ~AbstractFactory() {}
    
    static AbstractFactory* get();

    T* make( const std::string& aCreatorId, const std::string& aId, const swatch::core::XParameterSet& params );

    T* make( const swatch::core::XParameterSet& params );

    class CreatorInterface {
    public:
        virtual T* operator()( const std::string& aId, const swatch::core::XParameterSet& params ) = 0;
    };
    

private:
    /**
     * Basic creator class
     * 
     * @tparam D Product type, must be derived from T
     */
    template<typename D>
    class BasicCreator : public CreatorInterface {
    public:
        virtual T* operator() ( const std::string& aId, const swatch::core::XParameterSet& params );
    };
    
    template<typename K>
    bool add(const std::string& aCreatorName);
    
    private:
        
    static AbstractFactory* me_;
    boost::unordered_map<std::string, boost::shared_ptr<CreatorInterface> > creators_;

};

/**
 * Factory helper class
 * 
 * @tparam A Base product type
 * @tparam D Derived product type
 */
template< typename A, typename D >
struct ClassRegistrationHelper {
    ClassRegistrationHelper(const std::string& aClassName) {
        AbstractFactory<A>::get()->template add< typename AbstractFactory<A>::template BasicCreator<D> > (aClassName );
    }
};

/**
 * Factory helper class
 * 
 * @tparam A Base product type
 * @tparam K Creator type
 */
template< typename A, typename K >
struct CreatorRegistrationHelper {
    CreatorRegistrationHelper(const std::string& aCreatorName) {
        AbstractFactory<A>::get()->template add< K >(aCreatorName);
    }
};

/**
 * Factory helper class
 * 
 * @tparam A Base product type
 * @tparam D Derived product type
 */
template< typename A, typename D >
struct ClassRegistrationHelper2g {
    static bool initialised_;
    
    static bool init(const std::string& aClassName) {
         AbstractFactory<A>::get()->template add< typename AbstractFactory<A>::template BasicCreator<D> > ( aClassName );
         return true;
    }
};

/**
 * Factory helper class
 * 
 * @tparam A Base product type
 * @tparam K Creator type
 */
template< typename A, typename K >
struct CreatorRegistrationHelper2g {
    static bool initialised_;
    
    static bool init(const std::string& aCreatorName) {
         AbstractFactory<A>::get()->template add< K > ( aCreatorName );
         return true;
    }
};


} // namespace core
} // namespace swatch

#include "AbstractFactory.hxx"


#endif	/* __SWATCH_CORE_ABSTRACTFACTORY_HPP__ */

