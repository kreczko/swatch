
#ifndef SWATCH_MP7_MP7NULLALGOPROCESSOR_HPP
#define SWATCH_MP7_MP7NULLALGOPROCESSOR_HPP


#include "swatch/mp7/MP7Processor.hpp"


// Forward declarations
namespace mp7 {
class MP7Controller;
}


namespace swatch {
namespace mp7 {


class MP7NullAlgoProcessor: public MP7Processor {
public:
    MP7NullAlgoProcessor(const std::string& id, const swatch::core::XParameterSet& aPars);

    ~MP7NullAlgoProcessor();
};

} // namespace mp7 
} // namespace swatch


#endif

