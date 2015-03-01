/**
 * @file    xoperators.hpp
 * @author  Alessandro Thea
 * @brief   Streamers for xdata classes
 * @date    January 2015
 *
 */
#ifndef __SWATCH_CORE_XOPERATORS_HPP__
#define __SWATCH_CORE_XOPERATORS_HPP__

#include "xdata/Serializable.h"
#include "xdata/SimpleType.h"
#include "xdata/String.h"
#include "Utilities.hpp"

namespace xdata {
std::ostream& operator<< (std::ostream& stream, const xdata::Serializable& ser);

/*
 * xdata::SimpleType operators
 */

/*
 * operator==
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

 /*
 * operator==
 */
int operator==(const xdata::String & a, const xdata::String & b);

int operator==(const xdata::String & a, const std::string & b);

int operator==(const xdata::String & a, const char* b);

int operator==(const std::string & a, const xdata::String & b);

int operator==(const char* & a, const xdata::String b);

}

#endif /* __SWATCH_CORE_XOPERATORS_HPP__ */