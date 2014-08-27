/* 
 * File:   TestSuite.hpp
 * Author: ale
 *
 * Created on July 14, 2014, 6:39 PM
 */

#ifndef _swatch_test_testsuite_hpp_
#define	_swatch_test_testsuite_hpp_

#include <string>

namespace swatch {
namespace test {

//! base class for swatch test units
class TestUnit {
public:
    virtual bool run() = 0;
    virtual ~TestUnit() {}
protected:
    TestUnit( const std::string name ) : mName(name) {}
    
    const std::string& name() { return mName; }
private:
    const std::string mName;
    
};

class SystemExploreTest : public TestUnit {
public:
    SystemExploreTest( );
    virtual ~SystemExploreTest() {}
    
    virtual bool run();
};

class JsonBuildTest : public TestUnit {
public:
    JsonBuildTest( );
    virtual ~JsonBuildTest() {}
    
    virtual bool run();    
};

}
}



#endif	/* _swatch_test_testsuite_hpp_ */

