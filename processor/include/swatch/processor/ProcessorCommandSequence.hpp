/**
 * @file    ProcessorCommandSequence.hpp
 * @author  arose
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_ProcessorCommandSequence_HPP__
#define __SWATCH_CORE_ProcessorCommandSequence_HPP__

#include "swatch/core/CommandSequence.hpp"


namespace swatch {
namespace processor {

  class ProcessorCommandSequence : public core::CommandSequence {

  public:
    /// Constructor
    ProcessorCommandSequence( const std::string& aId );

    /// Destructor
    virtual ~ProcessorCommandSequence();
  
  private:
    std::vector< std::string >* setTables();

  };

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_ProcessorCommandSequence_HPP__ */
