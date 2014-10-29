
#include <boost/test/unit_test.hpp>

// Swathc Headers
//#include "swatch/processor/Connection.hpp"

// Namespace resolution
using std::cout;
using std::endl;

BOOST_AUTO_TEST_SUITE( ProcessorTestSuite )
        
class DummyDriver {
public:

    DummyDriver(const std::string& name) : name(name) {
        cout << "DummyDriver " << name << " is alive!" << endl;
    }

    DummyDriver(const DummyDriver& other) : name(other.name) {
        cout << "DummyDriver " << name << " is copied!" << endl;
    }

    virtual ~DummyDriver() {
        cout << "DummyDriver " << name << " is dead!" << endl;
    }

    std::string name;
};


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(ConnectionTest) {
//    using namespace swatch::processor;
//
//    Connection* i = Connection::make(5);
//    Connection* a = Connection::make(DummyDriver("alpha"));
//    Connection* b = Connection::make(new DummyDriver("beta"));
//
//    BOOST_CHECK( *(i->get<int>()) == 5 );
//    BOOST_CHECK( a->get<DummyDriver>()->name == "alpha" );
//    BOOST_CHECK( b->get<DummyDriver>()->name == "beta" );


}
        
        
        
        
BOOST_AUTO_TEST_SUITE_END() // ProcessorTestSuite