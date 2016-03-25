#include "swatch/core/xoperators.hpp"

namespace xdata {

std::ostream& operator<< (std::ostream& aStream, const xdata::Serializable& aSer)
{
    try {
        aStream << aSer.toString();
    } catch ( const xdata::exception::Exception& ) { 
        aStream << "<" << swatch::core::demangleName(typeid(aSer).name()) << ">";
    }
    return aStream;
}

/*
 * xdata::String
 */

 /*
 * operator==
 */

int operator==(const std::string & aStr1, const xdata::String & aStr2)
{
    return (aStr2 == aStr1);
}

int operator==(const char* & aStr1, const xdata::String& aStr2)
{
    return (aStr2 == aStr1);
}

}