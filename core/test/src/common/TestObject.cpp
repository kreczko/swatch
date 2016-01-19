// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
#include "boost/test/test_tools.hpp"

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/test/DummyObject.hpp"

// Boost Headers
#include <boost/assign.hpp>
#include <boost/foreach.hpp>


using namespace swatch::core::test;

namespace swatch {
namespace core {
namespace test{
      
struct TestFamily{
  TestFamily();
  ~TestFamily();
  
  DummyObject* granpa;
  DummyObject* parentA;
  DummyObject* kidA1;
  DummyObject* kidA2;
  DummyObject* parentB;
  DummyObject* kidB1;
  DummyObject* kidB2;
};

TestFamily::TestFamily() : 
  granpa(new DummyObject("granpa")),
  parentA(new DummyObject("parentA")),
  kidA1(new DummyObject("kidA1")),
  kidA2(new DummyObject("kidA2")),
  parentB(new DummyObject("parentB")),
  kidB1(new DummyObject("kidB1")),
  kidB2(new DummyObject("kidB2"))
{
  granpa->add(parentA);
  parentA->add(kidA1);
  parentA->add(kidA2);
  granpa->add(parentB);
  parentB->add(kidB1);
  parentB->add(kidB2);
}

TestFamily::~TestFamily()
{
  delete granpa;
}



BOOST_AUTO_TEST_SUITE( ObjectTestSuite )


BOOST_AUTO_TEST_CASE(ObjectIdEmpty)
{
  // Should throw if ID string is empty
  BOOST_CHECK_THROW( Object(""), InvalidObjectId );
  BOOST_CHECK_THROW( DummyObject(""), InvalidObjectId );
  
  BOOST_CHECK_NO_THROW( Object("validId") );
  BOOST_CHECK_NO_THROW( DummyObject("validId") );
}


BOOST_AUTO_TEST_CASE(ObjectIdContainsDots)
{
  // Should throw if ID string contains one or more dots (regardless of location)
  BOOST_CHECK_THROW( Object("."), InvalidObjectId );
  BOOST_CHECK_THROW( Object(".anId"), InvalidObjectId );
  BOOST_CHECK_THROW( Object("an.Id"), InvalidObjectId );
  BOOST_CHECK_THROW( Object("anId."), InvalidObjectId );
  
  BOOST_CHECK_THROW( DummyObject("."), InvalidObjectId );
  BOOST_CHECK_THROW( DummyObject(".anId"), InvalidObjectId );
  BOOST_CHECK_THROW( DummyObject("an.Id"), InvalidObjectId );
  BOOST_CHECK_THROW( DummyObject("anId."), InvalidObjectId );
}


BOOST_AUTO_TEST_CASE(ObjectGetTests)
{
    using namespace swatch::core;
    using namespace swatch::core::test;
    
    TestFamily family;
    
    // 1) Check access to immediate children (via pointer comparison)
    BOOST_CHECK_EQUAL( & family.granpa->getObj("parentA"), family.parentA );
    BOOST_CHECK_EQUAL( & family.parentA->getObj("kidA1"), family.kidA1 );
    BOOST_CHECK_EQUAL( & family.parentA->getObj("kidA2"), family.kidA2 );
    BOOST_CHECK_EQUAL( & family.parentB->getObj("kidB1"), family.kidB1 );
    BOOST_CHECK_EQUAL( & family.parentB->getObj("kidB2"), family.kidB2 );
    
    // 2) Check access to children of children (via pointer comparison)
    BOOST_CHECK_EQUAL( & family.granpa->getObj("parentA.kidA1"), family.kidA1 );
    BOOST_CHECK_EQUAL( & family.granpa->getObj("parentA.kidA2"), family.kidA2 );
    BOOST_CHECK_EQUAL( & family.granpa->getObj("parentB.kidB1"), family.kidB1 );
    BOOST_CHECK_EQUAL( & family.granpa->getObj("parentB.kidB2"), family.kidB2 );
    
    // 3) Check that throws correctly when invalid ID given
    BOOST_CHECK_THROW( family.granpa->getObj("invalid_object_id"), std::runtime_error);
    BOOST_CHECK_THROW( family.granpa->getObj("parentA.invalid_object_id"), std::runtime_error);
    
    // ...
    //TODO: Here, or in other test cases ... Check type, typeName, getPaths, template<T>getObj, getChildrenOfType
    //TODO: Update to BOOST_FIXTURE_TEST_CASE ???
} 


BOOST_AUTO_TEST_CASE(ObjectAddTests)
{
  DummyObject anObject("parent");
  
  // Setup: Add a dummy child
  DummyObject* child = new DummyObject("child");
  BOOST_CHECK_THROW( anObject.getObj("child"), std::runtime_error);
  anObject.add(child);
  BOOST_CHECK_EQUAL( & anObject.getObj("child"), child);
  
  // Check that Object::add throws when try to add another child with same ID
  DummyObject child2("child");
  BOOST_CHECK_THROW(anObject.add(&child2), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(ObjectIdPathTests)
{
  using namespace swatch::core;
  using namespace swatch::core::test;
  
  TestFamily family;
  
  BOOST_CHECK_EQUAL( family.granpa->getId(), "granpa");
  BOOST_CHECK_EQUAL( family.granpa->getPath(), "granpa");
  
  BOOST_CHECK_EQUAL( family.parentA->getId(), "parentA");
  BOOST_CHECK_EQUAL( family.parentB->getId(), "parentB");
  BOOST_CHECK_EQUAL( family.parentA->getPath(), "granpa.parentA");
  BOOST_CHECK_EQUAL( family.parentB->getPath(), "granpa.parentB");
  
  BOOST_CHECK_EQUAL( family.kidA1->getId(), "kidA1");
  BOOST_CHECK_EQUAL( family.kidA2->getId(), "kidA2");
  BOOST_CHECK_EQUAL( family.kidB1->getId(), "kidB1");
  BOOST_CHECK_EQUAL( family.kidB2->getId(), "kidB2");
  BOOST_CHECK_EQUAL( family.kidA1->getPath(), "granpa.parentA.kidA1");
  BOOST_CHECK_EQUAL( family.kidA2->getPath(), "granpa.parentA.kidA2");
  BOOST_CHECK_EQUAL( family.kidB1->getPath(), "granpa.parentB.kidB1");
  BOOST_CHECK_EQUAL( family.kidB2->getPath(), "granpa.parentB.kidB2");
}



BOOST_AUTO_TEST_CASE(ObjectChildrenTests)
{
     using namespace swatch::core;
     using namespace swatch::core::test;
     
     TestFamily family;
     
     // 1) Check children returned from granpa are correct
     std::vector<std::string> expectedIds = {"parentA", "parentB"};
     std::vector<std::string> returnedIds = family.granpa->getChildren();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 2) Check children returned from parentA are correct
     expectedIds = {"kidA1", "kidA2"};
     returnedIds = family.parentA->getChildren();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 3) Check children returned from parentB are correct
     expectedIds = {"kidB1", "kidB2"};
     returnedIds = family.parentB->getChildren();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 4) Check children returned from kids are correct
     BOOST_CHECK( family.kidA1->getChildren().empty() );
     BOOST_CHECK( family.kidA2->getChildren().empty() );
     BOOST_CHECK( family.kidB1->getChildren().empty() );
     BOOST_CHECK( family.kidB2->getChildren().empty() );
     
}

BOOST_AUTO_TEST_CASE(ObjectDescendantsTests)
{
     using namespace swatch::core;
     using namespace swatch::core::test;
     
     TestFamily family;
     
     // 1) Check descendants returned from granpa are correct
     std::vector<std::string> expectedIds = {"parentA", "parentA.kidA1", "parentA.kidA2", "parentB", "parentB.kidB1", "parentB.kidB2"};
     std::vector<std::string> returnedIds = family.granpa->getDescendants();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 2) Check descendants returned from parentA are correct
     expectedIds = {"kidA1", "kidA2"};
     returnedIds = family.parentA->getDescendants();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 3) Check children returned from parentB are correct
     expectedIds = {"kidB1", "kidB2"};
     returnedIds = family.parentB->getDescendants();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 4) Check children returned from kids are correct
     BOOST_CHECK( family.kidA1->getDescendants().empty() );
     BOOST_CHECK( family.kidA2->getDescendants().empty() );
     BOOST_CHECK( family.kidB1->getDescendants().empty() );
     BOOST_CHECK( family.kidB2->getDescendants().empty() );
     
}



//TODO
/*
BOOST_AUTO_TEST_CASE(ObjectViewTest) {  
  
    SimpleView* uncle = new SimpleView("uncle");
    
    uncle->add(kidA1, "nA1");
    uncle->add(kidA2, "nA2");    
    
    uncle->add(kidB1, "nB1");
    uncle->add(kidB2, "nB2");    

    
    nephews->add(kidA)
    
    granpa->add(nephews);
    
    

}
*/


BOOST_AUTO_TEST_SUITE_END() // ObjectTestSuite
        

} //namespace test
} //namespace core
} //namespace swatch
 
