// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
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


//____________________________________________________________________________//
BOOST_AUTO_TEST_CASE(ParameterSetTest) {

    // artificial scope to check destruction
    {
        using namespace boost::assign;
        using namespace swatch::core;

        ParameterSet pars;

        pars.set("alpha", "beta");
        BOOST_CHECK(pars.get<std::string>("alpha") == std::string("beta") );
        
//        BOOST_CHECK_MESSAGE(pars.get<std::string>("alpha") == std::string("beta1"), // #6 continues on error
        
        pars.insert("gamma", 5)("delta", "omega");
        BOOST_CHECK(pars.get<int>("gamma") == 5 );
        BOOST_CHECK(pars.get<std::string>("delta") == std::string("omega") );

        BOOST_REQUIRE(pars.size() == 3);
        
        pars.insert("eta", 10.5)("phi", 0x0);
        BOOST_CHECK(pars.get<double>("eta") == 10.5 );
        BOOST_CHECK(pars.get<int>("phi") == 0x0 );

        std::vector<int> intvector(3, 9);
        pars.set("vec", intvector);
        BOOST_CHECK(pars.get< std::vector<int> >("vec") == intvector );
        
        // Should fail here
        //  pars.set("zada",new float[10]);

        pars.set("dummyobj", Dummy("plain"));
        BOOST_CHECK(pars.get< Dummy >("dummyobj") == Dummy("plain") );
        // This will not compile
        // pars.set("dummy*", new Dummy("pointer") );

        ParameterSet subpars;

        subpars.insert("a", 1)("b", 2)("c", 3);
        pars.set("dummyset", subpars);
        BOOST_CHECK( pars.get< ParameterSet >("dummyset").names() == subpars.names() );
        BOOST_CHECK( pars.get< ParameterSet >("dummyset").get<int>("a") == subpars.get<int>("a") );

        std::vector<ParameterSet> vpars;
        vpars.push_back(subpars);
        pars.set("vset", vpars);
        BOOST_CHECK( pars.get< std::vector<ParameterSet> >("vset").size() == vpars.size() );

        boost::shared_ptr<Dummy> shr_ptr(new Dummy("shared_dummy"));
        pars.set("dummy_shr", shr_ptr);
        BOOST_CHECK( pars.get< boost::shared_ptr<Dummy> >("dummy_shr") == shr_ptr );

        BOOST_CHECK( pars.find("eta") != pars.end() );
        
        BOOST_CHECK( pars.get("doesntexist","abcd") == std::string("abcd") );
        BOOST_CHECK( pars.find("doesntexist") == pars.end() );
    }
}


BOOST_AUTO_TEST_CASE(ObjectTest) {
    using namespace boost::assign;
    using namespace swatch::core;
    using namespace swatch::core::test;
    
    SimpleObject* granpa = new SimpleObject("granpa");
    
    ParameterSet pset;

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
