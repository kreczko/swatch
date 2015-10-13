#ifndef __SWATCH_MP7_MP7MMCCOMMANDS_HPP__
#define __SWATCH_MP7_MP7MMCCOMMANDS_HPP__

#include "swatch/core/Command.hpp"


namespace swatch {
namespace mp7 {


class UploadFirmwareCommand : public ::swatch::core::Command {
public:
  UploadFirmwareCommand(const std::string& aId);
  virtual ~UploadFirmwareCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};

class DeleteFirmwareCommand : public ::swatch::core::Command {
public:
  DeleteFirmwareCommand(const std::string& aId);
  virtual ~DeleteFirmwareCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};

class RebootFPGACommand : public ::swatch::core::Command {
public:
  RebootFPGACommand(const std::string& aId);
  virtual ~RebootFPGACommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};

class HardResetCommand : public ::swatch::core::Command {
public:
  HardResetCommand(const std::string& aId);
  virtual ~HardResetCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};

class ScanSDCommand : public ::swatch::core::Command {
public:
  ScanSDCommand(const std::string& aId);
  virtual ~ScanSDCommand();
  virtual State code(const ::swatch::core::XParameterSet& params);

};


} //end ns mp7
} //end ns swatch


#endif /* __SWATCH_MP7_MP7MMCCOMMANDS_HPP__ */
