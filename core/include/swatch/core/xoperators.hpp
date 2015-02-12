#ifndef __SWATCH_CORE_XOPERATORS_HPP__
#define __SWATCH_CORE_XOPERATORS_HPP__

#include "xdata/SimpleType.h"


/*
 * xdata::SimpleType operators
 */
template<typename T>
int operator==(const xdata::SimpleType<T> & a, const xdata::SimpleType<T> & b) {
    return const_cast<xdata::SimpleType<T>&>(a).operator ==(b);
}

template<typename T>
int operator==(const xdata::SimpleType<T> & a, const T & b) {
    return const_cast<xdata::SimpleType<T>&>(a).operator ==(b);
}

template<typename T>
int operator==(const T & a, const xdata::SimpleType<T> & b) {
    return const_cast<xdata::SimpleType<T>&>(b).operator ==(a);
}

/*
 * xdata::String
 */
int operator==(const xdata::String & a, const xdata::String & b) {
    return const_cast<xdata::String &>(a).operator ==(b);
}

int operator==(const xdata::String & a, const std::string & b) {
    return const_cast<xdata::String &>(a).operator ==(b);
}

int operator==(const xdata::String & a, const char* b) {
    return const_cast<xdata::String &>(a).operator ==(b);
}

int operator==(const std::string & a, const xdata::String & b) {
    return const_cast<xdata::String &>(b).operator ==(a);
}

int operator==(const char* & a, const xdata::String b) {
    return const_cast<xdata::String &>(b).operator ==(a);
}

#endif /* __SWATCH_CORE_XOPERATORS_HPP__ */