/**
 * @file    ProcessorConfigSequence.hpp
 * @author  arose
 * @brief   
 * @date    May 2015
 *
 */

#ifndef __SWATCH_CORE_ProcessorConfigSequence_HPP__
#define __SWATCH_CORE_ProcessorConfigSequence_HPP__

#include "swatch/core/ConfigSequence.hpp"


namespace swatch {
namespace processor {

  class ProcessorConfigSequence : public core::ConfigSequence {

  public:
    /// Constructor
    ProcessorConfigSequence( const std::string& aId );

    /// Destructor
    virtual ~ProcessorConfigSequence();
  
  private:
    std::vector< std::string >* setTables();

  };

} /* namespace core */
} /* namespace swatch */

#endif /* __SWATCH_CORE_ProcessorConfigSequence_HPP__ */
