#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/XParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/AbstractFactory.hpp"
#include "swatch/core/test/DummyObject.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// Namespace resolution
//using namespace swatch::core;


// Type definition         
typedef swatch::core::AbstractFactory<swatch::core::Object> ObjFactory;
typedef swatch::core::test::DummyObject DummyObject;

// Standard factory registration macros
#define SWATCH_TEST_REGISTER_OBJ( classname ) _SWATCH_ABSTRACT_REGISTER_CLASS( swatch::core::Object, classname )
#define SWATCH_TEST_REGISTER_OBJCREATOR( creatorname ) _SWATCH_ABSTRACT_REGISTER_CREATOR( swatch::core::Object, creatorname )


namespace swatch {
namespace core {
namespace test {

// Dummy Creator interface
/*
class SimpleCreator : public ObjFactory::CreatorInterface {
public:
    virtual swatch::core::Object* operator()(const std::string& aId, const swatch::core::XParameterSet& params);
};
*/
class DummyCreator : public ObjFactory::CreatorInterface {
public:
    virtual swatch::core::Object* operator()( const swatch::core::AbstractStub& aStub );
};

swatch::core::Object*
DummyCreator::operator ()( const swatch::core::AbstractStub& aStub ) {
    swatch::core::test::DummyObject* so = new swatch::core::test::DummyObject(aStub);
    so->setValue(1234.5678);
    return so;
}

}
}
}


SWATCH_TEST_REGISTER_OBJ(swatch::core::test::DummyObject);

SWATCH_TEST_REGISTER_OBJCREATOR(swatch::core::test::DummyCreator)


BOOST_AUTO_TEST_SUITE( FactoryTestSuite )

BOOST_AUTO_TEST_CASE( TestFactory ) {
    
    ObjFactory* f = ObjFactory::get();
    swatch::core::AbstractStub d1("d1");    
//    swatch::core::XParameterSet none;
    swatch::core::Object* obj;
    obj = f->make<swatch::core::Object>("swatch::core::test::DummyObject",d1);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    BOOST_CHECK( dynamic_cast<DummyObject*>(obj) != 0x0 );
    
    delete obj;
    
    obj = f->make<swatch::core::Object>("swatch::core::test::DummyCreator",d1);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    
    DummyObject* so = dynamic_cast<DummyObject*>(obj);
    
    BOOST_CHECK( so != 0x0 );
    
    BOOST_CHECK( so->getValue() == 1234.5678 );
    
}
        
BOOST_AUTO_TEST_SUITE_END() // CoreTestSuite

