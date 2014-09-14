
#include "swatch/pyswatch/converters.hpp"

#include <boost/python.hpp>
#include <boost/any.hpp>
#include <boost/python/handle.hpp>
#include <boost/python/errors.hpp>



namespace bpy = boost::python;


// Map initialisation
Converter_boost_any_to_pyobj::ConvertersMap Converter_boost_any_to_pyobj::_converters = Converter_boost_any_to_pyobj::makemap();

bool
Converter_boost_any_to_pyobj::compare::operator()(const std::type_info* a, const std::type_info* b) const {
    return a->before(*b);
}

Converter_boost_any_to_pyobj::ConvertersMap
Converter_boost_any_to_pyobj::makemap() {
    ConvertersMap type_map;
    type_map.insert(std::make_pair(&typeid (std::string), &any_tostring));
    type_map.insert(std::make_pair(&typeid (short), &any_toint<short>));
    type_map.insert(std::make_pair(&typeid (int), &any_toint<int>));
    type_map.insert(std::make_pair(&typeid (long), &any_toint<long>));
    type_map.insert(std::make_pair(&typeid (ushort), &any_touint<ushort>));
    type_map.insert(std::make_pair(&typeid (uint), &any_touint<uint>));
    type_map.insert(std::make_pair(&typeid (ulong), &any_touint<ulong>));
    type_map.insert(std::make_pair(&typeid (float), &any_tofloat<float>));
    type_map.insert(std::make_pair(&typeid (double), &any_tofloat<double>));
    return type_map;
}

PyObject*
Converter_boost_any_to_pyobj::any_tostring(const boost::any& a) {
    const std::string& x = boost::any_cast<std::string>(a);
    return ::PyString_FromStringAndSize(
            x.data(),
            static_cast<ssize_t> (x.size())
            );
}

PyObject*
Converter_boost_any_to_pyobj::convert(const boost::any& a) {

    // Custom conversion for builtin types
    // As in...
    // boost/python/converter/builtin_converters.hpp
    // Unfortunately builtin convertes dont seem to be accessible
    ConvertersMap::const_iterator it = _converters.find(&(a.type()));
    if ( it != _converters.end() ) {
        std::cout << "Found in mymap : " << it->first->name() << std::endl;
        return (*(it->second))(a);
    } else {
        // But here we can get fancy
        boost::python::type_info info(a.type());
        const boost::python::converter::registration* reg = boost::python::converter::registry::query(info);

        if (reg == NULL) {
            // Better to throw here
            // Copied from libs/python/src/converter/registry.cpp
            // registration::to_python
            bpy::handle<> msg(
                    ::PyString_FromFormat(
                    "No to_python (by-value) converter found for C++ type: %s"
                    , info.name()
                    )
                    );

            PyErr_SetObject(PyExc_TypeError, msg.get());

            bpy::throw_error_already_set();
        }

        const void *ptr = boost::unsafe_any_cast<void *>(&a);
        PyObject* pyobj = reg->to_python(ptr);
        return pyobj;
    }
}