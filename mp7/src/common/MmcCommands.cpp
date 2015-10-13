
#include "swatch/mp7/MmcCommands.hpp"


#include <fstream>

#include "boost/algorithm/string/join.hpp"

#include "xdata/Integer.h"
#include "xdata/String.h"

#include "mp7/MP7Controller.hpp"
#include "mp7/MmcController.hpp"
#include "mp7/MmcPipeInterface.hpp"

#include "swatch/mp7/MP7Processor.hpp"


namespace swatch {
namespace mp7 {


// --------------------------------------------------------
UploadFirmwareCommand::UploadFirmwareCommand(const std::string& aId) :
Command(aId, xdata::Integer()) {
  registerParameter("localfile", xdata::String(""));
  registerParameter("sdfile", xdata::String(""));
}

// --------------------------------------------------------
UploadFirmwareCommand::~UploadFirmwareCommand() {
}

// --------------------------------------------------------
::swatch::core::Command::State
UploadFirmwareCommand::code(const ::swatch::core::XParameterSet& params) {
  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();

  std::string localfile = params.get<xdata::String>("localfile").value_;
  std::string sdfile = params.get<xdata::String>("sdfile").value_;
  std::ifstream firmware(localfile.c_str());

  if (localfile.empty() || sdfile.empty() || !firmware.good()) {
    setStatusMsg("No filepath or filename given, or file does not exist");
    return kError;
  }

  setProgress(0., "Uploading firmware image to uSD card ...");

  ::mp7::MmcController mmcController(mp7->driver().hw());
  mmcController.copyFileToSD(localfile, sdfile);

  setStatusMsg("Upload completed");
  return kDone;
}

// --------------------------------------------------------
DeleteFirmwareCommand::DeleteFirmwareCommand(const std::string& aId) :
Command(aId, xdata::Integer()) {
  registerParameter("sdfile", xdata::String(""));
}

// --------------------------------------------------------
DeleteFirmwareCommand::~DeleteFirmwareCommand() {
}

// --------------------------------------------------------
::swatch::core::Command::State
DeleteFirmwareCommand::code(const ::swatch::core::XParameterSet& params) {
  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();

  std::string sdfile = params.get<xdata::String>("sdfile").value_;

  if (sdfile.empty()) {
    setStatusMsg("Please provide a filename to delete");
    return kError;
  }

  if (!sdfile.compare("GoldenImage.bin")) {
    setStatusMsg("One must *not* delete the Golden Image!");
    return kError;
  }

  setProgress(0., "Deleting firmware image from uSD card ...");

  ::mp7::MmcController mmcController(mp7->driver().hw());
  mmcController.deleteFileFromSD(sdfile);

  //need to check if file has actually been deleted

  setStatusMsg("Delete Firmware Completed");

  return kDone;
}


// --------------------------------------------------------
RebootFPGACommand::RebootFPGACommand(const std::string& aId) :
Command(aId, xdata::Integer()) {
  registerParameter("sdfile", xdata::String(""));
}


// --------------------------------------------------------
RebootFPGACommand::~RebootFPGACommand() {
}


// --------------------------------------------------------
::swatch::core::Command::State
RebootFPGACommand::code(const ::swatch::core::XParameterSet& params) {
  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();
  std::string sdfile = params.get<xdata::String>("sdfile").value_;

  if (sdfile.empty()) {
    setStatusMsg("Please provide a firmware filename to reboot FPGA");
    return kError;
  }

  setProgress(0., "Rebooting FPGA ...");

  ::mp7::MmcController mmcController(mp7->driver().hw());
  mmcController.rebootFPGA(sdfile);

  ::mp7::MmcPipeInterface mmcNode(mp7->driver().hw().getNode< ::mp7::MmcPipeInterface>("uc"));
  std::string rebootfile = mmcNode.GetTextSpace();

  if (rebootfile.compare(sdfile)) {
    setStatusMsg("FPGA did not reboot with the firmware image provided. Please check filename!");
    return kError;
  }

  setStatusMsg("Reboot FPGA Completed");

  return kDone;
}

// --------------------------------------------------------
HardResetCommand::HardResetCommand(const std::string& aId) :
Command(aId, xdata::Integer()) {
}


// --------------------------------------------------------
HardResetCommand::~HardResetCommand() {
}


// --------------------------------------------------------
::swatch::core::Command::State
HardResetCommand::code(const ::swatch::core::XParameterSet& params) {
  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();
  ::mp7::MmcController mmcController(mp7->driver().hw());

  setProgress(0., "Performing Hard Reset of the board ...");

  mmcController.hardReset();

  setStatusMsg("Hard Reset Completed");

  return kDone;
}

// --------------------------------------------------------
ScanSDCommand::ScanSDCommand(const std::string& aId) :
Command(aId, xdata::String()) {
}

// --------------------------------------------------------
ScanSDCommand::~ScanSDCommand() {
}

// --------------------------------------------------------
::swatch::core::Command::State
ScanSDCommand::code(const ::swatch::core::XParameterSet& params) {
  ::swatch::mp7::MP7Processor* mp7 = getParent< ::swatch::mp7::MP7Processor>();

  setProgress(0., "Scanning SD card...");

  ::mp7::MmcPipeInterface mmcNode(mp7->driver().hw().getNode< ::mp7::MmcPipeInterface>("uc"));
  std::vector<std::string> fileNames = mmcNode.ListFilesOnSD();

  std::string fileNameString = boost::algorithm::join(fileNames, ",  ");
  //for(int i=0;i<fileNames.size();++i){

  setStatusMsg("Scan SD complete!");
  setResult(xdata::String(fileNameString));

  return kDone;
}


} //end ns mp7
} //end ns swatch
