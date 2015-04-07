// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/test/SimpleObject.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>




BOOST_AUTO_TEST_SUITE( CoreTestSuite )


class Dummy {
public:
    Dummy( const std::string& id ) : name(id) { ++counts; BOOST_TEST_MESSAGE( " >> Dummy " << name << " lives! (" << counts << ")");}
    Dummy( const Dummy& o ) : name(o.name) { ++counts; BOOST_TEST_MESSAGE( ">> Dummy " << name << " copied (" << counts << ")");}
    virtual ~Dummy() { --counts; BOOST_TEST_MESSAGE( " >> Dummy " <<  name << " dies! (" << counts << ")"); }
    
    bool operator==( const Dummy& o ) const { return name == o.name; }
    std::string name;
    static uint32_t counts;
};

uint32_t Dummy::counts = 0;

BOOST_AUTO_TEST_CASE(ObjectTest) {
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::core::test;
    
    SimpleObject* granpa = new SimpleObject("granpa");
    
    SimpleObject* daddy = new SimpleObject("daddy");
    
    granpa->add(daddy);
    
    SimpleObject* kid1 = new SimpleObject("kid1");
    SimpleObject* kid2 = new SimpleObject("kid2");
    
    daddy->add(kid1);
    daddy->add(kid2);
    
    SimpleView* nephews = new SimpleView("newphews");
    
    nephews->add(kid1, "n1");
    nephews->add(kid1, "n2");    
    
    granpa->add(nephews);
    
    BOOST_TEST_MESSAGE("Testing access");
    
    BOOST_CHECK( granpa->getObj("daddy") ==  daddy );
    BOOST_CHECK( granpa->getObj("daddy.kid1") ==  kid1 );
    BOOST_CHECK( granpa->getObj("daddy.kid2") ==  kid2 );
    
    BOOST_CHECK( granpa->getObj("daddy")->getObj("kid1") ==  kid1 );



    delete granpa;
}

BOOST_AUTO_TEST_SUITE_END() // CoreTestSuite
