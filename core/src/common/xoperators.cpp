#include "swatch/core/xoperators.hpp"

namespace xdata {

std::ostream& operator<< (std::ostream& stream, const xdata::Serializable& ser) {

    try {
        stream << const_cast<xdata::Serializable&>(ser).toString();
    } catch ( const xdata::exception::Exception& ) { 
        stream << "<" << swatch::core::demangleName(typeid(ser).name()) << ">";
    }
    return stream;
}

/*
 * xdata::String
 */

 /*
 * operator==
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

}