// Boost Unit Test includes
#include <boost/test/unit_test.hpp>

// Class under test
// Make everything public
#define private public
#define protected public
#include "swatch/core/XParameterSet.hpp"
#undef private
#undef public

// Swatch Headers


// Xdaq Headers
#include <xdata/Integer.h>
#include <xdata/Float.h>
#include <xdata/String.h>
#include <xdata/Vector.h>
#include <xdata/Table.h>
#include <xdata/Bag.h>



// C++ Header
#include <iterator> //for std::ostream_iterator

struct MyBag {
    public:
      MyBag() : a_(0), b_(0) {

      }
      MyBag( const MyBag& other ) : a_(-999), b_(-999) {
        std::cout << "Copy!" << std::endl;
        a_ = other.a_;
        b_ = other.b_;
      }

    void registerFields(xdata::Bag<MyBag> *bag) {
        bag->addField("a", &a_);
        bag->addField("b", &b_);
      }

    xdata::Integer a_;
    xdata::Float   b_;
};


BOOST_AUTO_TEST_SUITE( XParsTestSuite )

//---
BOOST_AUTO_TEST_CASE(CloneIntTest) {
  using namespace swatch::core;

  // xdata::Integer
  XParameterSet::XCloner cint = XParameterSet::cloner_<xdata::Integer>;
  xdata::Integer* xint = new xdata::Integer(5);
  
  xdata::Integer* oint = static_cast<xdata::Integer*>(cint(xint));
  
  std::cout << "Int        : " << xint << " " << *xint << std::endl;
  std::cout << "Other Int  : " << oint << " " << *oint << std::endl;

  BOOST_CHECK( oint->equals(*xint) );

  delete oint;
  delete xint;
}


//---
BOOST_AUTO_TEST_CASE(CloneFloatTest) {
  using namespace swatch::core;

  // xdata::Float
  XParameterSet::XCloner cfloat = XParameterSet::cloner_<xdata::Float>;

  xdata::Float* xfloat = new xdata::Float(3.14);
  
  xdata::Float* ofloat = static_cast<xdata::Float*>(cfloat(xfloat));
  
  std::cout << "Float       : " << xfloat << " " << *xfloat << std::endl;
  std::cout << "Other Float : " << ofloat << " " << *ofloat << std::endl;

  BOOST_CHECK( ofloat->equals(*xfloat) );

  delete ofloat;
  delete xfloat;
}


//---
BOOST_AUTO_TEST_CASE(CloneStringTest) {
  using namespace swatch::core;

  // XParameterSet::XCloner<xdata::String> cstr;
  XParameterSet::XCloner cstr = XParameterSet::cloner_<xdata::String>;

  xdata::String* xstr = new xdata::String("aaa");
  
  xdata::String* ostr = static_cast<xdata::String*>(cstr(xstr));
  
  std::cout << "String       : " << xstr << " " << xstr->toString() << std::endl;
  std::cout << "Other String : " << ostr << " " << ostr->toString() << std::endl;

  BOOST_CHECK( *ostr == *xstr );

  delete ostr;
  delete xstr;
}

BOOST_AUTO_TEST_CASE(CloneVectorTest) {
    using namespace swatch::core;

  // XParameterSet::XCloner<xdata::String> cstr;
  XParameterSet::XCloner cvint = XParameterSet::cloner_<xdata::Vector<xdata::Integer> >;
  xdata::Vector<xdata::Integer>* xvec = new xdata::Vector<xdata::Integer>();

  xdata::Vector<xdata::Integer>* ovec = static_cast<xdata::Vector<xdata::Integer>*>(cvint(xvec));

  BOOST_CHECK( ovec->equals(*xvec) );

}

//---
BOOST_AUTO_TEST_CASE(CloneTableTest) {
  using namespace swatch::core;

  // xdata::Table
  XParameterSet::XCloner ctable = XParameterSet::cloner_<xdata::Table>;
  xdata::Table* xtable = new xdata::Table();
  xtable->addColumn("my int","int");
  xtable->addColumn("my float","float");

  xdata::Integer ti(10);
  xdata::Float tf(6.28);
  xtable->setValueAt(0,"my int", ti);
  xtable->setValueAt(1,"my float", tf);       
  xdata::Table* otable = static_cast<xdata::Table*>(ctable(xtable));

  std::cout << "Table       : " << xtable << " " << xtable->toString() << std::endl;
  std::cout << "Other Table : " << otable << " " << otable->toString() << std::endl;

  delete xtable;
  delete otable;
}


//---
BOOST_AUTO_TEST_CASE(CloneBagTest) {
  using namespace swatch::core;

  // xdata::Bag
  XParameterSet::XCloner cbag = XParameterSet::cloner_< xdata::Bag<MyBag> >;

  xdata::Bag<MyBag>* xbag = new xdata::Bag<MyBag>();
  xbag->bag.a_ = 3;
  xbag->bag.b_ = 11;

  xdata::Bag<MyBag>* obag = static_cast<xdata::Bag<MyBag>*>( cbag(xbag) );
  
  std::cout << "Bag       : " << xbag << " " << xbag->bag.a_.toString() << " " << xbag->bag.b_ << std::endl;
  std::cout << "Other Bag : " << obag << " " << obag->bag.a_.toString() << " " << obag->bag.b_ << std::endl;

  BOOST_CHECK( obag->equals(*xbag) );

  delete obag;
  delete xbag;
}


//---
BOOST_AUTO_TEST_CASE(XParameterSetTest) {
  using namespace swatch::core;

  
  XParameterSet pars;

  std::cout << "Count entries: " << pars.size() << std::endl;
  std::cout << "Count 'pippo': " << pars.count("pippo") << std::endl;
  
  pars.add("pippo", xdata::String("pluto"));
  
  xdata::Bag<MyBag>* b = new xdata::Bag<MyBag>();
  pars.adopt("danulo", b);

  std::cout << "Count entries after create/insert" << pars.size() << std::endl;
  std::cout << "Count 'pippo' again: " << pars.count("pippo") << std::endl;

  std::cout << "Get 'pippo' from pars: " << pars.get<xdata::String>("pippo").toString() << std::endl;

  // Assign pippo
  pars.get<xdata::String>("pippo") = "padulo";
  std::cout << "Check pars size " << pars.size() << std::endl;

  std::cout << "Get " << pars.get<xdata::String>("pippo").toString() << std::endl;
  
  BOOST_CHECK( pars.equals(pars) );

  XParameterSet pars2 = pars;
  std::cout << "Check pars2 size " << pars2.size() << std::endl;
  BOOST_CHECK( pars2.equals(pars) );
  

  std::set<std::string> names = pars.names();
  std::copy(
    names.begin(),
    names.end(),
    std::ostream_iterator<std::string>(std::cout, "\n")
    );
  
  std::cout << std::endl;

  std::cout << "Pop 'pippo' from pars2: " << std::endl;

  xdata::String* s = pars2.pop<xdata::String>("pippo");
  
  std::cout << " 'pippo': " << s->toString() << std::endl;
  std::cout << "Check pars2 size " << pars2.size() << std::endl;


  
}
BOOST_AUTO_TEST_SUITE_END() // XParsTestSuite
