/* 
 * File:   Parameters.hpp
 * Author: ale
 *
 * Created on July 20, 2014, 9:50 AM
 */

#ifndef __swatch_core_parameterset_hpp__
#define	__swatch_core_parameterset_hpp__

// C++ Headers
#include <set>
#include <iostream>

// Boost Headers
#include <boost/unordered_map.hpp>
#include <boost/any.hpp>
#include <boost/type_traits/is_pod.hpp>


#include "swatch/core/Utilities.hpp"
#include "swatch/core/exception.hpp"

namespace swatch {
namespace core {

class ParameterSet : private boost::unordered_map<std::string, boost::any> {
    typedef boost::unordered_map<std::string, boost::any> parent;

public:

    typedef boost::any ParameterBase;
    typedef boost::bad_any_cast bad_cast;

    using parent::value_type;
    using parent::iterator;
    using parent::const_iterator;
    using parent::find;
    using parent::begin;
    using parent::end;
    using parent::size;
    
    ParameterSet();

    virtual ~ParameterSet();

    bool has(const std::string& aKey) const;
    
    template<typename T>
    void set(const std::string& aKey, const T& aValue);
    
    void set(const std::string& aKey, const char* aValue);
    
    template<typename T>
    T& get(const std::string& aKey);

    template<typename T>
    const T& get(const std::string& aKey) const;
    
    template<typename T>
    const T& get(const std::string& aKey, const T& aDefault ) const;

    std::string get(const std::string& aKey, const char* aDefault ) const;
    
    std::set<std::string> names() const;

    const ParameterBase& get(const std::string& aKey) const;

    std::string asstring( const std::string& aKey ) const;
    
    // Test Stuff DANGEROUS
    class Inserter {
    public:

        template<typename T>
        Inserter& operator()(const std::string& aKey, const T& aValue);
        
        Inserter& operator()(const std::string& aKey, const char* aValue);
    private:

        Inserter(ParameterSet* ps);

        ParameterSet *mPS;

        friend class ParameterSet;

    };

    template<typename T>
    Inserter insert(const std::string& aKey, const T& aValue);
    
    Inserter insert(const std::string& aKey, const char* aValue);

private:

};

DEFINE_SWATCH_EXCEPTION(ParameterNotFound);

typedef ParameterSet Arguments;

}
}

#include "swatch/core/ParameterSet.hxx"

#endif	/* __swatch_core_parameterset_hpp__ */

