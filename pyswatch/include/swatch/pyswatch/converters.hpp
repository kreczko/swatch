/* 
 * File:   converters.hpp
 * Author: ale
 *
 * Created on September 12, 2014, 4:52 PM
 */

#ifndef CONVERTERS_HPP
#define	CONVERTERS_HPP

#include <string>
#include <map>
#include <Python.h>
#include <boost/any.hpp>


struct Converter_boost_any_to_pyobj {

    static PyObject* convert(const boost::any& a);
    
private:
    // Add here a static boost unordered map pointer of callbacks for basic types
    // and a static method
    // static PyObject* convert_builtin( const boost::any& a )
    // which finds the callback associated to the type and executes it.
    // interger and floats can be templated
    // need a static method that builds the map.
    // singleton style.

    struct compare;
    typedef PyObject* (any_converter) (const boost::any& a);
    typedef std::map<const std::type_info*, any_converter*, compare> ConvertersMap;
    
    struct compare {
        bool operator()(const std::type_info* a, const std::type_info* b) const;
    };

    static ConvertersMap _converters;

    static PyObject* any_tostring(const boost::any& a);
    static ConvertersMap makemap();
    
    template<typename T>
    static PyObject* any_toint(const boost::any& a) {
        T x = boost::any_cast<T>(a);
        return ::PyInt_FromLong(x);
    }
    
    template<typename T>
    static PyObject* any_touint(const boost::any& a) {
        T x = boost::any_cast<T>(a);
        return ::PyLong_FromUnsignedLong(x);
    }

    template<typename T>
    static PyObject* any_tofloat(const boost::any& a) {
        T x = boost::any_cast<T>(a);
        return ::PyFloat_FromDouble(x);
    }
};


#endif	/* CONVERTERS_HPP */

