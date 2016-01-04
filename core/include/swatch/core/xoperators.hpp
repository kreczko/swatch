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

std::ostream& operator<< (std::ostream& aStream, const xdata::Serializable& aSer);

/*
 * xdata::SimpleType operators
 */

/*
 * operator==
 */
template<typename T>
int operator==(const xdata::SimpleType<T> & aSer1, const xdata::SimpleType<T> & aSer2) {
    return const_cast<xdata::SimpleType<T>&>(aSer1).operator ==(aSer2);
}

template<typename T>
int operator==(const xdata::SimpleType<T> & aSer1, const T & aSer2) {
    return const_cast<xdata::SimpleType<T>&>(aSer1).operator ==(aSer2);
}

template<typename T>
int operator==(const T & aSer1, const xdata::SimpleType<T> & aSer2) {
    return const_cast<xdata::SimpleType<T>&>(aSer2).operator ==(aSer1);
}

/*
 * xdata::String
 */

 /*
 * operator==
 */
int operator==(const xdata::String & aStr1, const xdata::String & aStr2);

int operator==(const xdata::String & aStr1, const std::string & aStr2);

int operator==(const xdata::String & aStr1, const char* aStr2);

int operator==(const std::string & aStr1, const xdata::String & aStr2);

int operator==(const char* & aStr1, const xdata::String& bStr2);

}

#endif /* __SWATCH_CORE_XOPERATORS_HPP__ */