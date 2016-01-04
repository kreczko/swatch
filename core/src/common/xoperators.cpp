#include "swatch/core/xoperators.hpp"

namespace xdata {

std::ostream& operator<< (std::ostream& aStream, const xdata::Serializable& aSer)
{
    try {
        aStream << const_cast<xdata::Serializable&>(aSer).toString();
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
 
int operator==(const xdata::String & aStr1, const xdata::String & aStr2)
{
    return const_cast<xdata::String &>(aStr1).operator ==(aStr2);
}

int operator==(const xdata::String & aStr1, const std::string & aStr2)
{
    return const_cast<xdata::String &>(aStr1).operator ==(aStr2);
}

int operator==(const xdata::String & aStr1, const char* aStr2)
{
    return const_cast<xdata::String &>(aStr1).operator ==(aStr2);
}

int operator==(const std::string & aStr1, const xdata::String & aStr2)
{
    return const_cast<xdata::String &>(aStr2).operator ==(aStr1);
}

int operator==(const char* & aStr1, const xdata::String& aStr2)
{
    return const_cast<xdata::String &>(aStr2).operator ==(aStr1);
}

}