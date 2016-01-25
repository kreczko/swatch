#include "swatch/xml/XmlReader.hpp"

#include <iterator>

// external headers
#include "pugixml/pugixml.hpp"
#include <log4cplus/loggingmacros.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem.hpp>

namespace swatch {
namespace xml {

XmlReader::XmlReader() :
    mLogger(swatch::logger::Logger::getInstance("swatch.xml.XmlReader")) {
}

XmlReader::~XmlReader() {

}

const std::string XmlReader::readXmlConfig(const std::string& aFileName) {
  pugi::xml_document lXmlDoc, lTmpDoc, lNewDoc;
  loadFromFile(aFileName, lXmlDoc);
  std::string lErrorMsg("");
  if (!checkMainConfig(lXmlDoc, lErrorMsg))
    throw InvalidConfig("The given XML file is not a valid SWATCH XML main config: " + aFileName + "\n" + lErrorMsg);

  pugi::xml_node lDb = lXmlDoc.child("db");
  pugi::xml_node lTmpDb = lTmpDoc.append_child("db");
  pugi::xml_node lNewDb = lNewDoc.append_child("db");

  for (pugi::xml_node lKey = lDb.first_child(); lKey; lKey = lKey.next_sibling()) {
    pugi::xml_node lTmpKey = lTmpDb.append_child("key");
    lTmpKey.append_attribute("id") = lKey.attribute("id").value();

    mergeSubConfigs(lKey, lTmpKey, aFileName);

    pugi::xml_node lNewKey = lNewDb.append_child("key");
    lNewKey.append_attribute("id") = lKey.attribute("id").value();

    mergeTables(lTmpKey, lNewKey);

  }

  return docToString(lNewDoc);
}

void XmlReader::mergeSubConfigs(const pugi::xml_node& aKeyNode, pugi::xml_node& aNewKeyNode,
    const std::string& aMainConfigPath) const {
  for (pugi::xml_node lLoad = aKeyNode.first_child(); lLoad; lLoad = lLoad.next_sibling()) {
    // load sub config
    std::string lSubConfigFile = lLoad.attribute("module").value();
    lSubConfigFile = normaliseSubConfigPath(aMainConfigPath, lSubConfigFile);
    pugi::xml_document lSubConfig;
    loadFromFile(lSubConfigFile, lSubConfig);
    // check sub config
    std::string lErrorMsg("");
    if (!checkSubConfig(lSubConfig, lErrorMsg))
      throw InvalidConfig("The given XML file is not a valid SWATCH XML sub-config: " + lSubConfigFile + "\n" + lErrorMsg);
    // import modules
    pugi::xml_node lModule = lSubConfig.child("module");
    for (pugi::xml_node lSubTag = lModule.first_child(); lSubTag; lSubTag = lSubTag.next_sibling()) {
      aNewKeyNode.append_copy(lSubTag);
    }
  }
}

const std::string XmlReader::normaliseSubConfigPath(const std::string& aMainConfigPath,
    const std::string& aSubConfigPath) const {
  if (boost::starts_with(aSubConfigPath, "/")) {
    // absolute path, do nothing
    return aSubConfigPath;
  } else {
    std::string lAbsolutePath = boost::filesystem::absolute((aMainConfigPath)).parent_path().string();
    return lAbsolutePath + "/" + aSubConfigPath;
  }
}

void XmlReader::mergeTables(const pugi::xml_node& aKeyNode, pugi::xml_node& aNewKeyNode) const {
  // this can be probably simplified
  for (pugi::xml_node lChild = aKeyNode.first_child(); lChild; lChild = lChild.next_sibling()) {
    // check if node has been already added
    pugi::xml_node lNewChild = aNewKeyNode.find_child_by_attribute(lChild.name(), "id", lChild.attribute("id").value());
    if (lNewChild) {
      for (pugi::xml_node lLeaf = lChild.first_child(); lLeaf; lLeaf = lLeaf.next_sibling()) {
        if (strcmp(lLeaf.name(), "state") == 0) {
          // merge states
          pugi::xml_node lNewLeaf = lNewChild.find_child_by_attribute("state", "id", lLeaf.attribute("id").value());
          for (pugi::xml_node lStateLeaf = lLeaf.first_child(); lStateLeaf; lStateLeaf = lStateLeaf.next_sibling()) {
            lNewLeaf.append_copy(lStateLeaf);
          }
        } else {
          lNewChild.append_copy(lLeaf);
        }
      }
    } else {
      aNewKeyNode.append_copy(lChild);
    }
  }

}

const std::string XmlReader::docToString(const pugi::xml_document& aDoc) const {
  XmlToStringWriter lWriter;
  aDoc.save(lWriter);
  return lWriter.mResult;
}

void XmlReader::XmlToStringWriter::write(const void* aData, size_t aSize) {
  mResult += std::string(static_cast<const char*>(aData), aSize);
}

bool XmlReader::checkMainConfig(const pugi::xml_document& aMainConfig, std::string& aErrorMsg) const {
  // top level node is db
  pugi::xml_node lDb = aMainConfig.child("db");
  // only one <db> tag per config
  bool lResult = std::distance(aMainConfig.children("db").begin(), aMainConfig.children("db").end()) == 1;
  if(!lResult) aErrorMsg += "More than one <db> tag detected\n";
  // the next level is one or more <key>
  for (pugi::xml_node lKey = lDb.first_child(); lKey; lKey = lKey.next_sibling()) {
    bool lIsKeyTag = strcmp(lKey.name(), "key") == 0;
    if (!lIsKeyTag) aErrorMsg += "A non <key> detected as child of <db>: <" +  std::string(lKey.name()) + ">\n";
    lResult = lResult && lIsKeyTag;
    // a <key> tag should only contain <load> tags
    for (pugi::xml_node lLoad = lKey.first_child(); lLoad; lLoad = lLoad.next_sibling()) {
      bool lIsLoadTag = strcmp(lLoad.name(), "load") == 0;
      if (!lIsLoadTag) aErrorMsg += "A non <load> detected as child of <key>: <" +  std::string(lLoad.name()) + ">\n";
      lResult = lResult && lIsLoadTag;
    }
  }

  return lResult;
}

bool XmlReader::checkSubConfig(const pugi::xml_document& aSubConfig, std::string& aErrorMsg) const {
  // top level node is module
  pugi::xml_node lModule = aSubConfig.child("module");
  // only one <db> tag per config
  bool lResult = std::distance(aSubConfig.children("module").begin(), aSubConfig.children("module").end()) == 1;
  if(!lResult) aErrorMsg += "More than one <module> tag detected\n";
  // the next level is one or more <table> or <disable> tag
  for (pugi::xml_node lSubTag = lModule.first_child(); lSubTag; lSubTag = lSubTag.next_sibling()) {
    std::string lSubTagName(lSubTag.name());
    bool lTableOrDisableTag = lSubTagName == "table" || lSubTagName == "disable";
    if (!lTableOrDisableTag) aErrorMsg += "A non <table> or <disable> detected as child of <module>: <" + lSubTagName + ">\n";
    lResult = lResult && lTableOrDisableTag;
    if (lSubTagName == "table") {
      // no load tags in <table>
      for (pugi::xml_node lTableEntry = lSubTag.first_child(); lTableEntry; lTableEntry = lTableEntry.next_sibling()) {
        bool lIsLoadTag  = strcmp(lTableEntry.name(), "load") == 0;
        if (lIsLoadTag) aErrorMsg += "A <load> detected as child of <table>: <" +  std::string(lTableEntry.name()) + ">\n";
        lResult = lResult && (!lIsLoadTag);
      }
    }
  }

  return lResult;
}

void XmlReader::loadFromFile(const std::string& aFilePath, pugi::xml_document& aDocToLoadInto) const {
  pugi::xml_parse_result lLoadResult = aDocToLoadInto.load_file(aFilePath.c_str());
  if (!lLoadResult) {
    LOG4CPLUS_ERROR(mLogger, "Error reading XML file '" << aFilePath << "'. Details: " << lLoadResult.description());
    throw XmlFileError("Error reading XML file '" + aFilePath + "' : " + lLoadResult.description());
  }
}

}
}
