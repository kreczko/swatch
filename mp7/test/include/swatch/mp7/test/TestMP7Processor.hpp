/* 
 * File:   TestMP7Processor.hpp
 * Author: ale
 *
 * Created on December 8, 2015, 2:19 PM
 */

#ifndef __SWATCH_MP7_TESTMP7PROCESSOR_HPP__
#define	__SWATCH_MP7_TESTMP7PROCESSOR_HPP__

#include "swatch/mp7/MP7NullAlgoProcessor.hpp"
#include "swatch/core/Command.hpp"
#include "swatch/mp7/MGTCommands.hpp"
#include "swatch/mp7/ChannelCommandCore.hpp"

namespace swatch {
namespace mp7 {
namespace test {

class TestMP7Processor : public MP7NullAlgoProcessor {
public:
  TestMP7Processor( const swatch::core::AbstractStub& aStub );
  virtual ~TestMP7Processor();
private:

};

class PrintRxDescriptors : public swatch::core::Command {
public:

  PrintRxDescriptors(const std::string& aId, core::ActionableObject& aResource);

  virtual State code(const core::XParameterSet& aParams);

private:
  
  RxBufferCommandCore mCore;
};


class TMTAlignCommand : public AlignRxsToCommand {
public:
  TMTAlignCommand(const std::string& aId, swatch::core::ActionableObject& aActionable);
  virtual State code(const core::XParameterSet& aParams);

};

} // namespace test
} // namespace mp7
} // namespace swatch

#endif	/* __SWATCH_MP7_TESTMP7PROCESSOR_HPP__ */

