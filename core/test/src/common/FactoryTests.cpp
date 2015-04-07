#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/AbstractFactory.hpp"
#include "swatch/core/test/SimpleObject.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// Namespace resolution
//using namespace swatch::core;


// Type definition         
typedef swatch::core::AbstractFactory<swatch::core::Object> ObjFactory;
typedef swatch::core::test::SimpleObject SimpleObject;

// Standard factory registration macros
#define SWATCH_TEST_REGISTER_OBJ( classname ) _SWATCH_ABSTRACT_REGISTER_CLASS( swatch::core::Object, classname )
#define SWATCH_TEST_REGISTER_OBJCREATOR( creatorname ) _SWATCH_ABSTRACT_REGISTER_CREATOR( swatch::core::Object, creatorname )


namespace swatch {
namespace core {
namespace test {

// Dummy Creator interface
class SimpleCreator : public ObjFactory::CreatorInterface {
public:
    virtual swatch::core::Object* operator()(const std::string& aId, const swatch::core::XParameterSet& params);
};


swatch::core::Object*
SimpleCreator::operator ()(const std::string& aId, const swatch::core::XParameterSet& params) {
    swatch::core::test::SimpleObject* so = new swatch::core::test::SimpleObject(aId, params);
    so->setValue(1234.5678);
    return so;
}

}
}
}


SWATCH_TEST_REGISTER_OBJ(swatch::core::test::SimpleObject);

SWATCH_TEST_REGISTER_OBJCREATOR(swatch::core::test::SimpleCreator)

BOOST_AUTO_TEST_SUITE( CoreTestSuite )

BOOST_AUTO_TEST_CASE( FactoryTest ) {
    
    ObjFactory* f = ObjFactory::get();
        
    swatch::core::XParameterSet none;
    swatch::core::Object* obj;
    obj = f->make("swatch::core::test::SimpleObject","d1", none);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    BOOST_CHECK( dynamic_cast<SimpleObject*>(obj) != 0x0 );
    
    delete obj;
    
    obj = f->make("swatch::core::test::SimpleCreator","d1", none);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    
    SimpleObject* so = dynamic_cast<SimpleObject*>(obj);
    
    BOOST_CHECK( so != 0x0 );
    
    BOOST_CHECK( so->getValue() == 1234.5678 );
    
}
        
BOOST_AUTO_TEST_SUITE_END() // CoreTestSuite

