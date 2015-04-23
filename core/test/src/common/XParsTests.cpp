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
#include "swatch/core/xoperators.hpp"

// Xdaq Headers
#include <xdata/Integer.h>
#include <xdata/Float.h>
#include <xdata/String.h>
#include <xdata/Vector.h>
#include <xdata/Table.h>
#include <xdata/Bag.h>
#include <xdata/Boolean.h>

// Boost Headers
#include "boost/foreach.hpp"

// C++ Header
#include <iterator>
#include <boost/test/test_tools.hpp> //for std::ostream_iterator

struct MyBag {
    public:
      MyBag() : a_(0), b_(0) {

      }
      MyBag( const MyBag& other ) : a_(-999), b_(-999) {
        // std::cout << "Copy!" << std::endl;
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

BOOST_AUTO_TEST_CASE( XBoolean ) {
  xdata::Boolean* b = new xdata::Boolean(true);
  
  bool c = *b;
  BOOST_CHECK( c );
  
  b->setValue(xdata::Boolean(false));

  c = *b;
  BOOST_CHECK( not c );

} 


//---
BOOST_AUTO_TEST_CASE(AddAdoptSetGetTest) {
  using namespace swatch::core;
  
  XParameterSet pars;

  // std::cout << "Count entries: " << pars.size() << std::endl;
  // std::cout << "Count 'aString': " << pars.has("aString") << std::endl;
  
  pars.add("aString", xdata::String("something"));
  
  xdata::Bag<MyBag>* b = new xdata::Bag<MyBag>();
  pars.adopt("aBag", b);

  BOOST_CHECK_EQUAL(pars.size(), 2);
  BOOST_CHECK_EQUAL(pars.has("aString"), true);
  BOOST_CHECK_EQUAL(pars.get<xdata::String>("aString").toString(), "something");

  // Assign aString
  pars.get<xdata::String>("aString") = "dummy";

  BOOST_CHECK_EQUAL(pars.size(), 2);

  BOOST_CHECK_EQUAL(pars.get<xdata::String>("aString").toString(), "dummy");
  BOOST_CHECK_EQUAL(pars.get("aString").toString(), "dummy");
  BOOST_CHECK_EQUAL(pars["aString"].toString(), "dummy");
  
  pars["aString"].setValue(xdata::String("dummier"));
  BOOST_CHECK_EQUAL(pars.get<xdata::String>("aString").toString(), "dummier");
  

  BOOST_CHECK_EQUAL(pars.get<xdata::String>("aString").toString(), "dummier");
  BOOST_CHECK_EQUAL(pars.get("aString").toString(), "dummier");
  BOOST_CHECK_EQUAL(pars["aString"].toString(), "dummier");

  BOOST_CHECK( pars.equals(pars) );

  XParameterSet pars2 = pars;
  BOOST_CHECK_EQUAL(pars2.size(), 2);

  // std::cout << "Check pars2 size " << pars2.size() << std::endl;
  BOOST_CHECK( pars2.equals(pars) );
  

  std::set<std::string> names = pars.keys();
  BOOST_CHECK_EQUAL(names.size(),2);
  BOOST_CHECK(names.find("aString") != names.end() );
  BOOST_CHECK(names.find("aBag") != names.end() );

  xdata::String* s = pars2.pop<xdata::String>("aString");
  BOOST_CHECK_EQUAL(s->toString(), "dummier");
  
  // std::cout << " 'aString': " << s->toString() << std::endl;
  // std::cout << "Check pars2 size " << pars2.size() << std::endl;
  BOOST_CHECK_EQUAL(pars2.size(), 1);
  
}

//---
BOOST_AUTO_TEST_CASE(FindTest) {
  using namespace swatch::core;

  XParameterSet pars;
  
  BOOST_CHECK_EQUAL(pars.has("any"),false);
  
  BOOST_CHECK_THROW(pars.get("any"), XParameterNotFound);

  BOOST_CHECK_THROW(pars["any"], XParameterNotFound);
  
}

//---
BOOST_AUTO_TEST_CASE(InsertTest) {
  using namespace swatch::core;
  
  XParameterSet pars;
  pars.insert("aString",  xdata::String("dummy"))("aFloat", xdata::Float(5.));
  BOOST_CHECK_EQUAL(pars.size(), 2);

  BOOST_CHECK(pars.get<xdata::String>("aString") == "dummy");
  BOOST_CHECK(pars.get<xdata::Float>("aFloat") == 5.);

}


//---
BOOST_AUTO_TEST_CASE(AddXPsetTest) {
  using namespace swatch::core;

  XParameterSet* child = new XParameterSet();
  child->add("1st", xdata::String("my first par"));
  child->add("2nd", xdata::Integer(99));
  child->add("3rd", xdata::Vector<xdata::Integer>());

  // std::cout << "Child content: " << child->toString() << std::endl;

  XParameterSet parent;
  parent.adopt("xps", child);
  BOOST_CHECK(&parent.get("xps") == child);
  BOOST_CHECK(parent.get<XParameterSet>("xps") == *child);


  BOOST_CHECK(parent.get<XParameterSet>("xps").get<xdata::String>("1st") == "my first par");

  // std::cout << "Parent content: " << parent.toString() << std::endl;


}


//---
BOOST_AUTO_TEST_CASE(OStreamTest) {
  using namespace swatch::core;

  std::ostringstream oss;
  
  oss.str("");
  const xdata::Integer aInt(5);
  oss << aInt;
  BOOST_CHECK_EQUAL(oss.str(),"5");
  
  oss.str("");
  const xdata::String aStr("dummy");
  oss << aStr;
  BOOST_CHECK_EQUAL(oss.str(),"dummy");

  oss.str("");
  const xdata::Bag<MyBag> aBag;
  oss << aBag;
  BOOST_CHECK_EQUAL(oss.str(),"<xdata::Bag<MyBag>>");

  oss.str("");
  const xdata::Serializable& aSer = aInt;
  oss << aSer;
  BOOST_CHECK_EQUAL(oss.str(),"5");
  
  XParameterSet xps;
  xps.add("aInt",aInt);
  xps.add("aString",aStr);
  xps.add("aBag",aBag);

  oss.str("");
  oss << xps.get("aInt");
  BOOST_CHECK_EQUAL(oss.str(),"5");
  
  oss.str("");
  oss << xps.get("aString");
  BOOST_CHECK_EQUAL(oss.str(),"dummy");



  const XParameterSet& cps = xps;
  BOOST_FOREACH( const std::string& k, xps.keys() ) {
    oss.str("");
    oss << k << " : " << cps[k];
    BOOST_TEST_MESSAGE(oss.str());
  }
}


//---
BOOST_AUTO_TEST_CASE(CastTest) {
  using namespace swatch::core;

  XParameterSet xps;
  xps.add("aString",xdata::String("aaa"));
  
  
  std::string s = xps.get<xdata::String>("aString");

}



//---
BOOST_AUTO_TEST_CASE(IterTest) {
  using namespace swatch::core;

  XParameterSet xps;
  xps.add("z",xdata::Integer(5));
  xps.add("f",xdata::String("dummy"));
  xps.add("a",xdata::Bag<MyBag>());

  BOOST_FOREACH( const std::string& k, xps.keys() ) {
    std::cout << k << " = "  << xps[k] <<  std::endl;
  }
}

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
  
  // std::cout << "Float       : " << xfloat << " " << *xfloat << std::endl;
  // std::cout << "Other Float : " << ofloat << " " << *ofloat << std::endl;

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
  
  // std::cout << "String       : " << xstr << " " << xstr->toString() << std::endl;
  // std::cout << "Other String : " << ostr << " " << ostr->toString() << std::endl;

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
  xtable->addColumn("my string","string");

  xdata::Integer ti(10);
  xdata::Float tf(6.28);
  xtable->setValueAt(0,"my int", ti);
  xtable->setValueAt(1,"my float", tf);       
  xdata::Table* otable = static_cast<xdata::Table*>(ctable(xtable));

  // std::cout << "Table       : " << xtable << " " << xtable->toString() << std::endl;
  // std::cout << "Other Table : " << otable << " " << otable->toString() << std::endl;

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
  
  // std::cout << "Bag       : " << xbag << " " << xbag->bag.a_.toString() << " " << xbag->bag.b_ << std::endl;
  // std::cout << "Other Bag : " << obag << " " << obag->bag.a_.toString() << " " << obag->bag.b_ << std::endl;

  BOOST_CHECK( obag->equals(*xbag) );

  delete obag;
  delete xbag;
}

//---
BOOST_AUTO_TEST_CASE(CloneXPsetTest) {
  using namespace swatch::core;

  // xdata::Bag
  XParameterSet::XCloner cpars = XParameterSet::cloner_< XParameterSet >;

  XParameterSet* xpars = new XParameterSet();
  xpars->add("a", xdata::Integer(3));
  xpars->add("b", xdata::Float(11));

  XParameterSet* opars = static_cast<XParameterSet*>( cpars(xpars) );
  
  // std::cout << "XPset       : " << xpars << " " << xpars->get("a").toString() << " " << xpars->get("b").toString() << std::endl;
  // std::cout << "Other XPset : " << opars << " " << opars->get("a").toString() << " " << opars->get("b").toString() << std::endl;

  BOOST_CHECK( opars->equals(*xpars) );

  delete opars;
  delete xpars;
}



BOOST_AUTO_TEST_SUITE_END() // XParsTestSuite
