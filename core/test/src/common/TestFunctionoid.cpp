// /*
//  * TestFunctionoid.cpp
//  *
//  *  Created on: 14 May 2015
//  *      Author: kreczko
//  */
// #include <boost/test/unit_test.hpp>
// 
// // swatch headers
// #include "swatch/core/XParameterSet.hpp"
// #include "swatch/core/Functionoid.hpp"
// #include "swatch/core/test/DummyFunctionoid.hpp"
// #include "swatch/logger/Log.hpp"
// 
// // XDAQ Headers
// #include <xdata/Serializable.h>
// #include <xdata/Integer.h>
// #include <xdata/String.h>
// 
// using namespace swatch::logger;
// namespace swatch {
// namespace core {
// namespace test {
// BOOST_AUTO_TEST_SUITE( FunctionoidTestSuite )
// BOOST_AUTO_TEST_CASE(AddDefault) {
//   LOG(kInfo) << "Running FunctionoidTestSuite/AddDefault";
//   DummyFunctionoid f("AddDefault");
//   f.registerParam("x", xdata::Integer(1));
//   const swatch::core::XParameterSet p = f.getDefaultParams();
// 
//   BOOST_CHECK_EQUAL(p.has("x"), true);
//   int i = p.get<xdata::Integer>("x");
//   BOOST_CHECK_EQUAL(i, int(1));
// }
// 
// BOOST_AUTO_TEST_CASE(ModifyDefault) {
//   LOG(kInfo) << "Running FunctionoidTestSuite/ModifyDefault";
//   DummyFunctionoid f("ModifyDefault");
//   f.registerParam("x", xdata::Integer(1));
//   f.registerParam("x", xdata::Integer(2));
//   const swatch::core::XParameterSet p = f.getDefaultParams();
// 
//   BOOST_CHECK_EQUAL(p.has("x"), true);
//   int i = p.get<xdata::Integer>("x");
//   BOOST_CHECK_EQUAL(i, int(2));
// }
// 
// BOOST_AUTO_TEST_CASE(MergeParameters) {
//   LOG(kInfo) << "Running FunctionoidTestSuite/MergeParameters";
//   DummyFunctionoid f("foo");
//   f.registerParam("x", xdata::Integer(1));
// 
//   swatch::core::XParameterSet params = XParameterSet();
//   params.add("y", xdata::Integer(3));
//   const swatch::core::XParameterSet p = f.mergeParametersWithDefaults(params);
// 
//   BOOST_CHECK_EQUAL(p.has("x"), true);
//   BOOST_CHECK_EQUAL(p.has("y"), true);
//   int x = p.get<xdata::Integer>("x");
//   int y = p.get<xdata::Integer>("y");
//   BOOST_CHECK_EQUAL(x, int(1));
//   BOOST_CHECK_EQUAL(y, int(3));
// }
// 
// BOOST_AUTO_TEST_CASE(MergeParametersNoOverwrite) {
//   LOG(kInfo) << "Running FunctionoidTestSuite/MergeParametersNoOverwrite";
//   DummyFunctionoid f("foo");
//   f.registerParam("x", xdata::Integer(1));
//   f.registerParam("z", xdata::Integer(5));
// 
//   swatch::core::XParameterSet params = XParameterSet();
//   params.add("x", xdata::Integer(2));
//   params.add("y", xdata::Integer(3));
//   const swatch::core::XParameterSet p = f.mergeParametersWithDefaults(params);
// 
//   BOOST_CHECK_EQUAL(p.has("x"), true);
//   BOOST_CHECK_EQUAL(p.has("y"), true);
//   BOOST_CHECK_EQUAL(p.has("z"), true);
//   int x = p.get<xdata::Integer>("x");
//   int y = p.get<xdata::Integer>("y");
//   BOOST_CHECK_EQUAL(x, int(2));
//   BOOST_CHECK_EQUAL(y, int(3));
// }
// 
// BOOST_AUTO_TEST_SUITE_END() // FunctionoidTestSuite
// } /* namespace test */
// } /* namespace core */
// } /* namespace swatch */
