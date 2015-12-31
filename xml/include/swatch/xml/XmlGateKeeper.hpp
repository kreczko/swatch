/**
 * @file    XmlGateKeeper.hpp
 * @author  arose
 * @brief   Guardian to the underworld that is the online database
 * @date    May 2015
 *
 */

#ifndef __SWATCH_XML_XmlGateKeeper_HPP__
#define __SWATCH_XML_XmlGateKeeper_HPP__

// C++ Headers
#include <string>

// Swatch Headers
#include "swatch/core/GateKeeper.hpp"
#include "swatch/xml/XmlSerializer.hpp"
#include "swatch/logger/Logger.hpp"

// external headers
#include "pugixml/pugixml.hpp"

namespace pugi {
class xml_node;
}

namespace swatch {
namespace xml {

class XmlGateKeeper: public core::GateKeeper {
public:

  /// Constructor
  XmlGateKeeper(const std::string& aFileName, const std::string& aKey);
  XmlGateKeeper(const pugi::xml_document& aXmlDoc, const std::string& aKey);

  /// Destructor
  virtual ~XmlGateKeeper();

private:

  void readXmlDocument(const pugi::xml_document& aXmlDoc, const std::string& aRunKey);

  std::pair<std::string, GateKeeper::Parameter_t> createParameter(pugi::xml_node& aEntry);
  std::pair<std::string, GateKeeper::ParametersTable_t> createTable(pugi::xml_node& aTable);

  std::pair<std::string, GateKeeper::MonitoringSetting_t> createMonitoringSetting(
      const pugi::xml_node& aEntry) const;
  std::pair<std::string, GateKeeper::SettingsTable_t> createSettingsTable(
      const pugi::xml_node& aTable) const;

  std::pair < std::string, GateKeeper::MasksTable_t> createMasksTable(const pugi::xml_node& aTable) const;

  std::string mFileName;
  XmlSerializer* mSerializer;
  log4cplus::Logger mLogger;
};

DEFINE_SWATCH_EXCEPTION(XmlFileError);

} /* namespace xml */
} /* namespace swatch */

#endif /* __SWATCH_XML_XmlGateKeeper_HPP__ */
