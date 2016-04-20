// SWATCH headers
#include "swatch/core/toolbox/IdSliceParser.hpp"
#include "swatch/logger/Logger.hpp"
//#include "swatch/processor/Utilities.hpp"
#include "swatch/xml/XmlSystem.hpp"
// only for now
#include "swatch/xml/XmlReader.hpp"

// external headers
#include "pugixml/pugixml.hpp"
#include <boost/unordered_map.hpp>
#include "boost/lexical_cast.hpp"
#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace xml {
namespace system {
const char* MAINTAG = "system";
const std::vector<std::string> SUBTAGS = { "creator", "crate", "processor", "daqttc-mgr", "link", "connected-fed" };

swatch::system::SystemStub xmlFileToSystemStub(const std::string& aFileName) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlFileToSystemStub"));
  LOG4CPLUS_DEBUG(lLogger, "Reading sytem XML file '" + aFileName + "'");
  pugi::xml_document lXmlDoc;
  detail::loadFromFile(aFileName, lXmlDoc);

  return xmlToSystemStub(lXmlDoc);
}

swatch::system::SystemStub xmlToSystemStub(const pugi::xml_document& aXmlDoc) {
  using namespace detail;
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlToSystemStub"));
  if (!detail::validateSystemXml(aXmlDoc)) {
    throw InvalidSystemDescription("The given XML is not a valid description of swatch::system::System");
  }

  pugi::xml_node lSystem = aXmlDoc.child(MAINTAG);
  pugi::xml_attribute lId = lSystem.attribute("id");

  swatch::system::SystemStub lStub(lId.value());
  for (pugi::xml_node lNode = lSystem.first_child(); lNode; lNode = lNode.next_sibling()) {
    std::string lTag(lNode.name());
    if (lTag == "creator") {
      lStub.creator = lNode.child_value();
    } else if (lTag == "crate") {
      lStub.crates.emplace_back(xmlToCrateStub(lNode));
    } else if (lTag == "processor") {
      swatch::processor::ProcessorStub lPStub(xmlToProcessorStub(lNode));
      lPStub.loggerName = lStub.id + "." + lPStub.id;
      lStub.processors.emplace_back(lPStub);
    } else if (lTag == "daqttc-mgr") {
      swatch::dtm::DaqTTCStub lDaqStub(xmlToDaqTTCStub(lNode));
      lDaqStub.loggerName = lStub.id + "." + lDaqStub.id;
      lStub.daqttcs.emplace_back(lDaqStub);
    } else if (lTag == "link") {
      lStub.links = xmlToLinkStubs(lNode);
    } else if (lTag == "connected-fed") {
      lStub.connectedFEDs = xmlToConnectedFeds(lNode);
    } else if (lTag == "excluded-boards") {
      //TODO: excluded boards
    } else {
      //TODO: unknown tag
    }
  } // end for loop

  return lStub;
}

namespace detail {
swatch::system::CrateStub xmlToCrateStub(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlToCrateStub"));
  std::string lId = aNode.attribute("id").value();
  std::string lDescription = aNode.child_value("description");
  std::string lLocation = aNode.child_value("location");

  swatch::system::CrateStub lStub(lId);
  lStub.description = lDescription;
  lStub.location = lLocation;

  return lStub;
}

swatch::processor::ProcessorStub xmlToProcessorStub(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlToProcessorStub"));
  pugi::xml_attribute lId = aNode.attribute("id");
  // one of each
  std::string lCreator = aNode.child_value("creator");
  std::string lHwType = aNode.child_value("hw-type");
  std::string lRole = aNode.child_value("role");
  std::string lCrate = aNode.child_value("crate");
  std::string lSlot = aNode.child_value("slot");
  // ==1 for processor, ==2 for DaqTTC-mgr
  std::string lUri = aNode.child_value("uri");
  std::string lAddrTable = aNode.child_value("address-table");
  // one or more
  swatch::processor::ProcessorStub lStub(lId.value());
  for (pugi::xml_named_node_iterator lTxPort = aNode.children("tx-port").begin();
      lTxPort != aNode.children("tx-port").end(); ++lTxPort) {
    std::string lName(lTxPort->attribute("name").value());
    std::string lPid(lTxPort->attribute("pid").value());
    pushBackPortStubs(lStub.txPorts, lName, lPid);
  }
  for (pugi::xml_named_node_iterator lTxPort = aNode.children("rx-port").begin();
      lTxPort != aNode.children("rx-port").end(); ++lTxPort) {
    std::string lName(lTxPort->attribute("name").value());
    std::string lPid(lTxPort->attribute("pid").value());
    pushBackPortStubs(lStub.rxPorts, lName, lPid);
  }
  lStub.creator = lCreator;
  lStub.hwtype = lHwType;
  lStub.role = lRole;
  lStub.crate = lCrate;
  lStub.slot = boost::lexical_cast<uint32_t>(lSlot);
  lStub.loggerName = lStub.id;

  lStub.uri = lUri;
  lStub.addressTable = lAddrTable;

  return lStub;
}

swatch::dtm::DaqTTCStub xmlToDaqTTCStub(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlToDaqTTCStub"));
  std::string lId = aNode.attribute("id").value();
  std::string lCreator = aNode.child_value("creator");
  std::string lRole = aNode.child_value("role");
  std::string lCrate = aNode.child_value("crate");
  std::string lSlot = aNode.child_value("slot");
  std::string lFedId = aNode.child_value("fed-id");

  swatch::dtm::DaqTTCStub lStub(lId);
  lStub.creator = lCreator;
  lStub.role = lRole;
  lStub.crate = lCrate;
  lStub.slot = boost::lexical_cast<uint32_t>(lSlot);
  lStub.fedId = boost::lexical_cast<uint16_t>(lFedId);
  lStub.loggerName = lStub.id;

  // TODO: move this to validation
  uint16_t lNUris = numberOfChildren(aNode, "uri");
  if (lNUris != 2) {
    std::string lMsg = "Expected 2 <uri> tags for <daqttc-mgr> but got " + boost::lexical_cast<std::string>(lNUris)
        + " instead.";
    LOG4CPLUS_ERROR(lLogger, lMsg);
    throw InvalidSystemDescription(lMsg);
  }
  pugi::xml_object_range<pugi::xml_named_node_iterator> lUris = aNode.children("uri");
  lStub.uriT1 = aNode.child("uri").child_value();
  lStub.uriT2 = aNode.child("uri").next_sibling("uri").child_value();

  uint16_t lNAddrTab = numberOfChildren(aNode, "address-table");
  if (lNAddrTab != 2) {
    std::string lMsg = "Expected 2 <address-table> tags for <daqttc-mgr> but got "
        + boost::lexical_cast<std::string>(lNAddrTab) + " instead.";
    LOG4CPLUS_ERROR(lLogger, lMsg);
    throw InvalidSystemDescription(lMsg);
  }
  pugi::xml_object_range<pugi::xml_named_node_iterator> lAddrTables = aNode.children("address-table");
  lStub.addressTableT1 = aNode.child("address-table").child_value();
  lStub.addressTableT2 = aNode.child("address-table").next_sibling("address-table").child_value();

  return lStub;
}

std::vector<swatch::system::LinkStub> xmlToLinkStubs(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.xmlToLinkStubs"));
  std::string lId = aNode.attribute("id").value();

  std::string lSource = aNode.child_value("from");
  std::string lDestination = aNode.child_value("to");
  std::string lSrcPortsStr = aNode.child_value("rx-port");
  std::string lDstPortsStr = aNode.child_value("tx-port");

  std::vector<std::string> lNames = core::toolbox::IdSliceParser::parse(lId);
  std::vector<std::string> lSrcPorts = core::toolbox::IdSliceParser::parse(lSrcPortsStr);
  std::vector<std::string> lDstPorts = core::toolbox::IdSliceParser::parse(lDstPortsStr);

  if (lSrcPorts.size() != lNames.size()) {
    std::ostringstream lMsg;
    lMsg << "Link Stub creation error: Link ids '" << lId << "' do not have the same multiplicity as source ids '"
        << lSource << "', '" << lSrcPortsStr << "'. N(ids)=" << lNames.size() << " N(srcs)=" << lSrcPorts.size();
    LOG4CPLUS_ERROR(lLogger, lMsg);
    throw InvalidSystemDescription(lMsg.str());
  }

  if (lDstPorts.size() != lNames.size()) {
    std::ostringstream lMsg;
    lMsg << "Link Stub creation error: Link ids '" << lId << "' do not have the same multiplicity as destination ids '"
        << lDestination << "', '" << lDstPortsStr << "'. N(ids)=" << lNames.size() << " N(srcs)=" << lDstPorts.size();
    LOG4CPLUS_ERROR(lLogger, lMsg);
    throw InvalidSystemDescription(lMsg.str());
  }

  std::vector<swatch::system::LinkStub> lStubs;
  for (size_t i = 0; i < lNames.size(); ++i) {
    swatch::system::LinkStub lStub(lNames.at(i));
    lStub.srcProcessor = lSource;
    lStub.dstProcessor = lDestination;
    lStub.srcPort = lSrcPorts.at(i);
    lStub.dstPort = lDstPorts.at(i);
    lStubs.push_back(lStub);
  }

  return lStubs;
}

bool validateSystemXml(const pugi::xml_document& aXmlDoc) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateSystemXml"));
  pugi::xml_node lSystem = aXmlDoc.child(MAINTAG);

// only one <system> tag per config
  bool lResult = std::distance(aXmlDoc.children(MAINTAG).begin(), aXmlDoc.children(MAINTAG).end()) == 1;
  if (!lResult) {
    LOG4CPLUS_ERROR(lLogger, "More than one <" + std::string(MAINTAG) + "> tag detected");
  }

  if (!hasAttr(lSystem, "id")) {
    lResult = false;
    LOG4CPLUS_ERROR(lLogger, "<" + std::string(MAINTAG) + "> tag has no 'id' attribute!");
  }

  typedef boost::unordered_map<std::string, unsigned int> Counter;
  Counter lCounter;

  for (pugi::xml_node lChild = lSystem.first_child(); lChild; lChild = lChild.next_sibling()) {
    std::string lName = lChild.name();
    if (!validateChildXml(lChild)) {
      lResult = false;
      LOG4CPLUS_ERROR(lLogger, "Something wrong with <system> child <" + lName + "> ");
    }
    lResult = lResult && validateChildXml(lChild);

    if (std::find(SUBTAGS.begin(), SUBTAGS.end(), lName) != SUBTAGS.end()) {
      lCounter[lName] += 1;
    } else {
      //TODO: what to do if you find "unknown tags
      LOG4CPLUS_WARN(lLogger, "<" + lName + "> tag is not known.");
      lCounter["UNKNOWN"] += 1;
    }
  }

  for (std::vector<std::string>::const_iterator lTag = SUBTAGS.begin(); lTag != SUBTAGS.end(); ++lTag) {
    bool lFound(false);
    for (Counter::const_iterator lIt = lCounter.begin(); lIt != lCounter.end(); ++lIt) {
      if (lIt->first == *lTag) {
        lFound = true;
        if (lIt->first == "creator" && lCounter["creator"] != 1) {
          lResult = false;
          LOG4CPLUS_ERROR(lLogger, "More than one <creator> tag detected");
        }
      }
    }
    if (!lFound) {
      lResult = false;
      LOG4CPLUS_ERROR(lLogger, "No <" + *lTag + "> tag found!");
    }
  }

  return lResult;
}

bool validateChildXml(const pugi::xml_node& aChildNode) {
  std::string lName = aChildNode.name();
  if (lName == "creator") {
    return validateCreatorXml(aChildNode);
  } else if (lName == "crate") {
    return validateCrateXml(aChildNode);
  } else if (lName == "processor") {
    return validateProcessorXml(aChildNode);
  } else if (lName == "daqttc-mgr") {
    return validateDAQTTCXml(aChildNode);
  } else if (lName == "link") {
    return validateLinkXml(aChildNode);
  } else if (lName == "connected-fed") {
    return true;
  } else {
    return true;
  }
  return true;
}

bool validateCreatorXml(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateCreatorXml"));
  bool lResult = true;

  if (strcmp(aNode.child_value(), "") == 0) {
    LOG4CPLUS_ERROR(lLogger, "<" << aNode.name() << "> tag is empty!");
    lResult = false;
  }

  return lResult;
}

bool validateCrateXml(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateCrateXml"));
  bool lResult = true;

  if (!hasAttr(aNode, "id")) {
    LOG4CPLUS_ERROR(lLogger, "<" << aNode.name() << "> tag has no 'id' attribute!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "location");
  lResult = lResult && childContentNonEmpty(aNode, "description");

  return lResult;
}

bool validateProcessorXml(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateProcessorXml"));
  bool lResult = true;
  if (!hasAttr(aNode, "id")) {
    LOG4CPLUS_ERROR(lLogger, "<" << aNode.name() << "> tag has no 'id' attribute!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "hw-type");
  lResult = lResult && childContentNonEmpty(aNode, "role");

  size_t lNUri = numberOfChildren(aNode, "uri");
  if (lNUri != 1) {
    LOG4CPLUS_ERROR(lLogger, "Require exactly 1 <uri> tag for <processor>, but found " << lNUri << "!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "uri");

  size_t lNAddrTab = numberOfChildren(aNode, "address-table");
  if (lNAddrTab != 1) {
    LOG4CPLUS_ERROR(lLogger, "Require exactly 1 <address-table> tag for <processor>, but found " << lNAddrTab << "!");
    lResult = false;
  }
  lResult = lResult && childContentNonEmpty(aNode, "address-table");
  lResult = lResult && childContentNonEmpty(aNode, "crate");
  lResult = lResult && childContentNonEmpty(aNode, "slot");

  lResult = lResult && hasAttr(aNode.child("rx-port"), "name");
  lResult = lResult && hasAttr(aNode.child("rx-port"), "pid");

  lResult = lResult && hasAttr(aNode.child("tx-port"), "name");
  lResult = lResult && hasAttr(aNode.child("tx-port"), "pid");

  return lResult;
}

bool validateDAQTTCXml(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateDAQTTCXml"));
  bool lResult = true;
  if (!hasAttr(aNode, "id")) {
    LOG4CPLUS_ERROR(lLogger, "<" << aNode.name() << "> tag has no 'id' attribute!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "role");

  size_t lNUri = numberOfChildren(aNode, "uri");
  if (lNUri != 2) {
    LOG4CPLUS_ERROR(lLogger, "Require exactly 2 <uri> tag for <daqttc-mgr>, but found " << lNUri << "!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "uri");

  size_t lNAddrTab = numberOfChildren(aNode, "address-table");
  if (lNAddrTab != 2) {
    LOG4CPLUS_ERROR(lLogger, "Require exactly 2 <address-table> tag for <daqttc-mgr>, but found " << lNAddrTab << "!");
    lResult = false;
  }
  lResult = lResult && childContentNonEmpty(aNode, "address-table");
  lResult = lResult && childContentNonEmpty(aNode, "crate");
  lResult = lResult && childContentNonEmpty(aNode, "slot");
  lResult = lResult && childContentNonEmpty(aNode, "fed-id");

  return lResult;
}

bool validateLinkXml(const pugi::xml_node& aNode) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.validateLinkXml"));
  bool lResult = true;

  if (!hasAttr(aNode, "id")) {
    LOG4CPLUS_ERROR(lLogger, "<" << aNode.name() << "> tag has no 'id' attribute!");
    lResult = false;
  }

  lResult = lResult && childContentNonEmpty(aNode, "from");
  lResult = lResult && childContentNonEmpty(aNode, "to");

  lResult = lResult && childContentNonEmpty(aNode, "tx-port");
  lResult = lResult && childContentNonEmpty(aNode, "rx-port");


  return lResult;
}

bool hasAttr(const pugi::xml_node& aNode, const std::string& aAttr) {
  pugi::xml_attribute lAttribute = aNode.attribute(aAttr.c_str());
  if (!lAttribute)
    return false;

  std::string lAttrValue(lAttribute.value());
  if (lAttrValue.empty())
    return false;

  return true;
}

bool childContentNonEmpty(const pugi::xml_node& aNode, const std::string& aChildName) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.system.childContentNonEmpty"));
  unsigned int i = 1;
  bool lResult(true);
  for (pugi::xml_named_node_iterator lChild = aNode.children(aChildName.c_str()).begin();
      lChild != aNode.children(aChildName.c_str()).end(); ++lChild) {
    if (strcmp(lChild->child_value(), "") == 0) {
      LOG4CPLUS_ERROR(lLogger, i << ". entry of <" << aChildName << "> tag for <" << aNode.name() << "> is empty!");
      lResult = false;
    }
  }

  return lResult;
}

//TODO: move this to general XML functions
void loadFromFile(const std::string& aFilePath, pugi::xml_document& aDocToLoadInto) {
  log4cplus::Logger lLogger(swatch::logger::Logger::getInstance("swatch.xml.loadFromFile"));
  pugi::xml_parse_result lLoadResult = aDocToLoadInto.load_file(aFilePath.c_str());
  if (!lLoadResult) {
    LOG4CPLUS_ERROR(lLogger, "Error reading XML file '" << aFilePath << "'. Details: " << lLoadResult.description());
    throw XmlFileError("Error reading XML file '" + aFilePath + "' : " + lLoadResult.description());
  }
}

size_t numberOfChildren(const pugi::xml_node& aNode) {
  return std::distance(aNode.children().begin(), aNode.children().end());
}

size_t numberOfChildren(const pugi::xml_node& aNode, const std::string& aChildName) {
  return std::distance(aNode.children(aChildName.c_str()).begin(), aNode.children(aChildName.c_str()).end());
}

void pushBackPortStubs(std::vector<swatch::processor::ProcessorPortStub>& aPortStubs, const std::string& aName,
    const std::string& aIndex) {
  std::vector<std::string> names = core::toolbox::IdSliceParser::parse(aName);
  std::vector<std::string> indices = core::toolbox::IdSliceParser::parse(aIndex);

  if (names.size() != indices.size())
    throw std::runtime_error(
        boost::lexical_cast<std::string>(names.size()) + " port names created from name \"" + aName
            + "\" using slice syntax, but " + boost::lexical_cast<std::string>(indices.size())
            + " indices created from \"" + aIndex + "\"");

  for (size_t i = 0; i < names.size(); i++) {

    swatch::processor::ProcessorPortStub b(names.at(i));
    b.number = boost::lexical_cast<unsigned>(indices.at(i));
    aPortStubs.push_back(b);
  }
}

} // detail
} // system
} // xml
} // swatch
