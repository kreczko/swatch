#include <boost/test/unit_test.hpp>

// Swatch Headers
#include "swatch/core/ParameterSet.hpp"
#include "swatch/core/Object.hpp"
#include "swatch/core/AbstractFactory.hpp"
#include "swatch/core/test/SimpleObject.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>

// Namespace resolution
//using namespace swatch::core;

BOOST_AUTO_TEST_SUITE( CoreTestSuite )

// Type definition         
typedef swatch::core::AbstractFactory<swatch::core::Object> ObjFactory;
typedef swatch::core::test::SimpleObject SimpleObject;

// Standard factory registration macros
#define SWATCH_TEST_REGISTER_OBJ( classname ) \
swatch::core::ClassRegistrationHelper< swatch::core::Object, classname > classname##ObjClassRegistrationHelper( #classname );

#define SWATCH_TEST_REGISTER_OBJCREATOR( creatorname ) \
swatch::core::CreatorRegistrationHelper< swatch::core::Object, creatorname > creatorname##ObjCreatorRegistrationHelper( #creatorname );


// Dummy Creator interface
class SimpleCreator : public ObjFactory::CreatorInterface {
public:
    virtual swatch::core::Object* operator()(const std::string& aId, const swatch::core::ParameterSet& aPSet);
};


swatch::core::Object*
SimpleCreator::operator ()(const std::string& aId, const swatch::core::ParameterSet& aPSet) {
    swatch::core::test::SimpleObject* so = new swatch::core::test::SimpleObject(aId, aPSet);
    so->setValue(1234.5678);
    return so;
}


SWATCH_TEST_REGISTER_OBJ(SimpleObject);

SWATCH_TEST_REGISTER_OBJCREATOR(SimpleCreator)

BOOST_AUTO_TEST_CASE( FactoryTest ) {
    
    ObjFactory* f = ObjFactory::get();
        
    swatch::core::Arguments none;
    swatch::core::Object* obj;
    obj = f->make("SimpleObject","d1", none);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    BOOST_CHECK( dynamic_cast<SimpleObject*>(obj) != 0x0 );
    
    delete obj;
    
    obj = f->make("SimpleCreator","d1", none);
    
    BOOST_CHECK( typeid(obj) == typeid(swatch::core::Object*) );
    
    
    SimpleObject* so = dynamic_cast<SimpleObject*>(obj);
    
    BOOST_CHECK( so != 0x0 );
    
    BOOST_CHECK( so->getValue() == 1234.5678 );
    
}
        
BOOST_AUTO_TEST_SUITE_END() // CoreTestSuite

