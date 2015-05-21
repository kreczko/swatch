// Boost Unit Test includes
#include <boost/test/unit_test.hpp>
#include "boost/test/test_tools.hpp"

// Swatch Headers
#include "swatch/core/Object.hpp"
#include "swatch/core/test/SimpleObject.hpp"

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
  
  SimpleObject* granpa;
  SimpleObject* parentA;
  SimpleObject* kidA1;
  SimpleObject* kidA2;
  SimpleObject* parentB;
  SimpleObject* kidB1;
  SimpleObject* kidB2;
};

TestFamily::TestFamily() : 
  granpa(new SimpleObject("granpa")),
  parentA(new SimpleObject("parentA")),
  kidA1(new SimpleObject("kidA1")),
  kidA2(new SimpleObject("kidA2")),
  parentB(new SimpleObject("parentB")),
  kidB1(new SimpleObject("kidB1")),
  kidB2(new SimpleObject("kidB2"))
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


BOOST_AUTO_TEST_CASE(ObjectGetTests) {
    using namespace swatch::core;
    using namespace swatch::core::test;
    
    TestFamily family;
    
    // 1) Check access to immediate children (via pointer comparison)
    BOOST_CHECK_EQUAL( family.granpa->getObj("parentA"), family.parentA );
    BOOST_CHECK_EQUAL( family.parentA->getObj("kidA1"), family.kidA1 );
    BOOST_CHECK_EQUAL( family.parentA->getObj("kidA2"), family.kidA2 );
    BOOST_CHECK_EQUAL( family.parentB->getObj("kidB1"), family.kidB1 );
    BOOST_CHECK_EQUAL( family.parentB->getObj("kidB2"), family.kidB2 );
    
    // 2) Check access to children of children (via pointer comparison)
    BOOST_CHECK_EQUAL( family.granpa->getObj("parentA.kidA1"), family.kidA1 );
    BOOST_CHECK_EQUAL( family.granpa->getObj("parentA.kidA2"), family.kidA2 );
    BOOST_CHECK_EQUAL( family.granpa->getObj("parentB.kidB1"), family.kidB1 );
    BOOST_CHECK_EQUAL( family.granpa->getObj("parentB.kidB2"), family.kidB2 );
    
    // 3) Check that throws correctly when invalid ID given
    BOOST_CHECK_THROW( family.granpa->getObj("invalid_object_id"), std::runtime_error);
    BOOST_CHECK_THROW( family.granpa->getObj("parentA.invalid_object_id"), std::runtime_error);
    
    // ...
    //TODO: Here, or in other test cases ... Check type, typeName, getPaths, template<T>getObj, getChildrenOfType
    //TODO: Update to BOOST_FIXTURE_TEST_CASE ???
} 


BOOST_AUTO_TEST_CASE(ObjectAddTests) {
  SimpleObject anObject("parent");
  
  // Setup: Add a dummy child
  SimpleObject* child = new SimpleObject("child");
  BOOST_CHECK_THROW( anObject.getObj("child"), std::runtime_error);
  anObject.add(child);
  BOOST_CHECK_EQUAL( anObject.getObj("child"), child);
  
  // Check that Object::add throws when try to add another child with same ID
  SimpleObject child2("child");
  BOOST_CHECK_THROW(anObject.add(&child2), std::runtime_error);
}


BOOST_AUTO_TEST_CASE(ObjectIdPathTests) {
  using namespace swatch::core;
  using namespace swatch::core::test;
  
  TestFamily family;
  
  BOOST_CHECK_EQUAL( family.granpa->id(), "granpa");
  BOOST_CHECK_EQUAL( family.granpa->path(), "granpa");
  
  BOOST_CHECK_EQUAL( family.parentA->id(), "parentA");
  BOOST_CHECK_EQUAL( family.parentB->id(), "parentB");
  BOOST_CHECK_EQUAL( family.parentA->path(), "granpa.parentA");
  BOOST_CHECK_EQUAL( family.parentB->path(), "granpa.parentB");
  
  BOOST_CHECK_EQUAL( family.kidA1->id(), "kidA1");
  BOOST_CHECK_EQUAL( family.kidA2->id(), "kidA2");
  BOOST_CHECK_EQUAL( family.kidB1->id(), "kidB1");
  BOOST_CHECK_EQUAL( family.kidB2->id(), "kidB2");
  BOOST_CHECK_EQUAL( family.kidA1->path(), "granpa.parentA.kidA1");
  BOOST_CHECK_EQUAL( family.kidA2->path(), "granpa.parentA.kidA2");
  BOOST_CHECK_EQUAL( family.kidB1->path(), "granpa.parentB.kidB1");
  BOOST_CHECK_EQUAL( family.kidB2->path(), "granpa.parentB.kidB2");
}


/*
BOOST_AUTO_TEST_CASE(ObjectChildrenTests) {
     using namespace swatch::core;
     using namespace swatch::core::test;
     
     TestFamily family;
     
     // 1) Check children returned from granpa are correct
     std::vector<std::string> expectedIds, returnedIds;
     expectedIds.push_back("parentA");
     expectedIds.push_back("parentB");
     returnedIds = family.granpa->getChildren();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 2) Check children returned from parentA are correct
     expectedIds.clear();
     expectedIds.push_back("kidA1");
     expectedIds.push_back("kidA2");
     returnedIds = family.parentA->getChildren();
     
     std::sort(expectedIds.begin(), expectedIds.end());
     std::sort(returnedIds.begin(), returnedIds.end());
     BOOST_CHECK_EQUAL_COLLECTIONS( returnedIds.begin(), returnedIds.end(), expectedIds.begin(), expectedIds.end() );
     
     // 3) Check children returned from parentB are correct
     expectedIds.clear();
     expectedIds.push_back("kidB1");
     expectedIds.push_back("kidB2");
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
*/

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
 
