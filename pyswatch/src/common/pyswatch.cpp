// Boost Headers
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/wrapper.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/to_python_converter.hpp>

#include <boost/python/converter/registry.hpp>
#include <boost/python/converter/registrations.hpp>
#include <boost/python/class.hpp>
#include <boost/python/handle.hpp>
#include <boost/python/errors.hpp>

#include "swatch/core/ParameterSet.hpp"
#include "swatch/pyswatch/converters.hpp"

// using namespace boost::python;
namespace bpy = boost::python;

namespace pyswatch {
/// Returns dummy ParameterSet instance for testing.

swatch::core::ParameterSet get_dummy_ParameterSet() {
    swatch::core::ParameterSet pset, subpset;
    subpset.insert("d", 15);
    pset.insert("a", "b")("c", 5)("e", subpset);
    pset.insert("x",123.4)("u",(uint32_t)0xdeadc0de);
    
    return pset;
}

void wrap_test_functions() {

    bpy::scope packageScope;
    std::string testModuleName(bpy::extract<const char*> (packageScope.attr("__name__")));

    testModuleName += ".test";
    char* testModuleCstr = new char [testModuleName.size() + 1];
    strcpy(testModuleCstr, testModuleName.c_str());

    // Make test sub-module ...
    bpy::object testModule(bpy::handle<> (bpy::borrowed(PyImport_AddModule(testModuleCstr)))); //< Enables "from mypackage.tests import <whatever>"
    packageScope.attr("tests") = testModule; //< Enables "from mypackage import tests"
    // Change to sub-module scope ...
    bpy::scope testScope = testModule;

    bpy::def("get_dummy_ParameterSet", get_dummy_ParameterSet);
}

// CONVERTERS

void register_converters() {
    bpy::to_python_converter< boost::any, Converter_boost_any_to_pyobj >();
}


// *** N.B: The argument of this BOOST_PYTHON_MODULE macro MUST be the same as the name of the library created, i.e. if creating library file my_py_binds_module.so , imported in python as:
//                import my_py_binds_module
//          then would have to put
//                BOOST_PYTHON_MODULE(my_py_binds_module)
//          Otherwise, will get the error message "ImportError: dynamic module does not define init function (initmy_py_binds_module)

BOOST_PYTHON_MODULE(_pyswatch) {
    using namespace boost::python;
    wrap_test_functions();
    register_converters();

    bpy::class_<swatch::core::ParameterSet>("ParameterSet")
            .def("get", static_cast<const swatch::core::ParameterSet::ParameterBase & (swatch::core::ParameterSet::*)(const std::string&) const> (&swatch::core::ParameterSet::get), bpy::return_value_policy<bpy::copy_const_reference>())
            .def("get2", static_cast<const swatch::core::ParameterSet::ParameterBase & (swatch::core::ParameterSet::*)(const std::string&) const> (&swatch::core::ParameterSet::get), bpy::return_internal_reference<>())
            .def("asstring", &swatch::core::ParameterSet::asstring)
            ;
}

}