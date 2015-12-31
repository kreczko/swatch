#include "swatch/xml/XmlGateKeeper.hpp"

#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <xdata/Integer.h>
#include <xdata/UnsignedInteger.h>
#include <xdata/String.h>
#include <xdata/Boolean.h>
#include <xdata/Float.h>
#include <xdata/Vector.h>

#include "swatch/logger/Log.hpp"
#include "swatch/core/Utilities.hpp"

#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace xml {
using namespace swatch::core;


//------------------------------------------------------------------------------------------------------------------
XmlGateKeeper::XmlGateKeeper(const std::string& aFileName, const std::string& aKey) :
  GateKeeper(aKey),
  mFileName(swatch::core::shellExpandPath(aFileName)),
  mSerializer(new XmlSerializer()),
  mLogger(swatch::logger::Logger::getInstance("swatch.xml.XmlGateKeeper"))
{
  pugi::xml_document lXmlDoc;
  pugi::xml_parse_result lLoadResult = lXmlDoc.load_file(mFileName.c_str());

  if (not lLoadResult) {
    LOG(swatch::logger::kError) << "Error reading XML file '" << aFileName
        << "'. Details: " << lLoadResult.description() << std::endl;
    throw XmlFileError(
        "Error reading XML file '" + aFileName + "' : "
            + lLoadResult.description());
  }

  readXmlDocument(lXmlDoc, aKey);
}


//------------------------------------------------------------------------------------------------------------------
XmlGateKeeper::XmlGateKeeper(const pugi::xml_document& aXmlDoc, const std::string& aRunKey) :
  GateKeeper(aRunKey),
  mFileName(""),
  mSerializer(new XmlSerializer()),
  mLogger(swatch::logger::Logger::getInstance("swatch.xml.XmlGateKeeper"))
{
 readXmlDocument(aXmlDoc, aRunKey);
}


//------------------------------------------------------------------------------------------------------------------
void XmlGateKeeper::readXmlDocument(const pugi::xml_document& aXmlDoc, const std::string& aRunKey)
{
  pugi::xml_node lRun(aXmlDoc.child("db").find_child_by_attribute("run", "key", aRunKey.c_str()));

  for (pugi::xml_node lTable(lRun.child("table")); lTable; lTable = lTable.next_sibling("table"))
  {
    std::pair < std::string, GateKeeper::ParametersTable_t > lParameterTable(createTable(lTable));
    add(lParameterTable.first, lParameterTable.second);

    std::pair < std::string, GateKeeper::SettingsTable_t > lSettingsTable(createSettingsTable(lTable));
    add(lSettingsTable.first, lSettingsTable.second);
    
    std::pair < std::string, GateKeeper::MasksTable_t> lMasksTable(createMasksTable(lTable));
    add(lMasksTable.first, lMasksTable.second);
  }
}


//------------------------------------------------------------------------------------------------------------------
XmlGateKeeper::~XmlGateKeeper()
{
}


//------------------------------------------------------------------------------------------------------------------
std::pair<std::string, GateKeeper::Parameter_t> XmlGateKeeper::createParameter(pugi::xml_node& aEntry)
{
  std::string lEntryId(aEntry.attribute("id").value());
  std::string lType(aEntry.attribute("type").value());
  std::string lValue(aEntry.child_value());
  xdata::Serializable* lSerializable(mSerializer->import(aEntry));

  return std::make_pair(lEntryId, GateKeeper::Parameter_t(lSerializable));
}


//------------------------------------------------------------------------------------------------------------------
std::pair<std::string, GateKeeper::ParametersTable_t> XmlGateKeeper::createTable(pugi::xml_node& aTable)
{
  std::string lTableId(aTable.attribute("id").value());
  ParametersTable_t lParameterTable(new Parameters_t());

  for (pugi::xml_node lEntry(aTable.child("entry")); lEntry; lEntry = lEntry.next_sibling("entry")) {
    std::pair<std::string, GateKeeper::Parameter_t> lParameter;
    try {
      lParameter = createParameter(lEntry);
    } catch (const std::exception& e) {
      std::string lEntryId(lEntry.attribute("id").value());
      std::string lMsg("Error parsing parameter '" + lEntryId + "' from table '" + lTableId + "'");
      LOG4CPLUS_FATAL(mLogger, lMsg + ": " << e.what());
      throw e;
    }

    if (lParameterTable->find(lParameter.first) != lParameterTable->end()) {
      throw ParameterWithGivenIdAlreadyExistsInTable("Parameter with ID '" + lParameter.first + "' already exists in table '" + lTableId + "' in file '" + mFileName + "'");
    }

    lParameterTable->insert(lParameter);
  }
  return std::make_pair(lTableId, lParameterTable);
}


//------------------------------------------------------------------------------------------------------------------
std::pair<std::string, GateKeeper::SettingsTable_t> XmlGateKeeper::createSettingsTable(
    const pugi::xml_node& aTable) const
{
  std::string lTableId(aTable.attribute("id").value());
  SettingsTable_t lSettingsTable(new MonitoringSettings_t());

  for (pugi::xml_node lEntry(aTable.child("state")); lEntry;
      lEntry = lEntry.next_sibling("state")) {
    std::string lStateId(lEntry.attribute("id").value());
    //metric + mon-obj
    for (pugi::xml_node lMetric(lEntry.child("metric")); lMetric; lMetric =
        lMetric.next_sibling("metric")) {
      std::pair<std::string, GateKeeper::MonitoringSetting_t> lSetting(
          createMonitoringSetting(lMetric));

      std::string lStatePath = lStateId + "." + lSetting.first;
      lSetting.first = lStatePath;
      if (lSettingsTable->find(lStatePath) != lSettingsTable->end()) {
        throw MSettingWithGivenIdAlreadyExistsInTable(
            "Monitoring setting with ID '" + lStatePath
                + "' already exists in table '" + lTableId + "' in file '"
                + mFileName + "'");
      }

      lSettingsTable->insert(lSetting);

    }

    for (pugi::xml_node lMonObj(lEntry.child("mon-obj")); lMonObj; lMonObj =
        lMonObj.next_sibling("mon-obj")) {
      std::pair<std::string, GateKeeper::MonitoringSetting_t> lSetting(
          createMonitoringSetting(lMonObj));

      std::string lStatePath = lStateId + "." + lSetting.first;
      lSetting.first = lStatePath;
      if (lSettingsTable->find(lStatePath) != lSettingsTable->end()) {
        throw MSettingWithGivenIdAlreadyExistsInTable(
            "Monitoring setting with ID '" + lStatePath
                + "' already exists in table '" + lTableId + "' in file '"
                + mFileName + "'");
      }

      lSettingsTable->insert(lSetting);
    }
  }
  return std::make_pair(lTableId, lSettingsTable);
}


//------------------------------------------------------------------------------------------------------------------
std::pair<std::string, GateKeeper::MonitoringSetting_t> XmlGateKeeper::createMonitoringSetting(const pugi::xml_node& aEntry) const 
{
  std::string lId(aEntry.attribute("id").value());
  std::string lStatus(aEntry.attribute("status").value());

  monitoring::Status lMonStatus(monitoring::StringToStatus.at(lStatus)); // add check
  GateKeeper::MonitoringSetting_t lMonSetting(new MonitoringSetting(lId, lMonStatus));
  return std::make_pair(lId, lMonSetting);
}

//------------------------------------------------------------------------------------------------------------------
std::pair<std::string, GateKeeper::MasksTable_t> XmlGateKeeper::createMasksTable(const pugi::xml_node& aTable) const
{
  std::string lTableId(aTable.attribute("id").value());
  MasksTable_t lMaskTable(new Masks_t());

  for (pugi::xml_node lEntry(aTable.child("mask")); lEntry; lEntry = lEntry.next_sibling("mask")) {

    std::string lId = lEntry.attribute("id").value();
    
    if (lMaskTable->find(lId) != lMaskTable->end())
      throw ParameterWithGivenIdAlreadyExistsInTable("Mask with ID '" + lId + "' already exists in table '" + lTableId + "' in file '" + mFileName + "'");

    lMaskTable->insert(lId);
  }
  return std::make_pair(lTableId, lMaskTable);
}


} // namespace core
} // namespace swatch
