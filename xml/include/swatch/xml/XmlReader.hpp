#ifndef __SWATCH_XML_XMLINCLUDE_HPP__
#define __SWATCH_XML_XMLINCLUDE_HPP__

// Standard headers
#include <string>
#include <vector>

// SWATCH headers
#include "swatch/core/exception.hpp"
#include "swatch/logger/Logger.hpp"

// external
#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {
/**
 * The XML reader is a helper class to read, verify and transform XML files.
 * The main config is expected to have the form
 * <db>
 *  <key id="MyDummyKey`>
 *    <load module="<path to sub config>" />
 *    ...
 *  </key>
 * </db>
 *
 * Where <path to sub config> is either an absolute path or it will be intepreted
 * as a relative path with respect to the location of the main config.
 */
class XmlReader {
public:
  XmlReader();
  ~XmlReader();

  /**
   * Reads the XML configuration from a file, loads sub configuration files,
   * checks for validity and returns the combined result.
   */
  const std::string readXmlConfig(const std::string& aFileName);

  /**
   * Checks if the main config has the correct structure:
   * <db>
   *   <key id="keyId>
   *     <load ... /> <!-- one or more -->
   *     ...
   *   </key>
   * </db>
   */
  bool checkMainConfig(const pugi::xml_document& aMainConfig, std::string& aErrorMsg) const;

  /**
   * Checks if a sub config has the correct structure:
   * <module>
   *   <table id="tableId> <!-- 0 or more -->
   *     <entry ... /> <!-- one or more -->
   *     ...
   *   </table>
   *   <disable id="system.brokenProcessor" /> <!-- 0 or more -->
   * </module>
   */
  bool checkSubConfig(const pugi::xml_document& aSubConfig, std::string& aErrorMsg) const;

  /**
   * Returns the string representation of a XML document
   */
  const std::string docToString(const pugi::xml_document& aDoc) const;

  /**
   * Takes a file path (aFilePath) and loads it into an XML document (aDocToLoadInto).
   * Throws an exception if file cannot be opened.
   */
  void loadFromFile(const std::string& aFilePath, pugi::xml_document& aDocToLoadInto) const;

private:
  log4cplus::Logger mLogger;

  struct XmlToStringWriter: pugi::xml_writer {
    std::string mResult;
    virtual void write(const void* aData, size_t aSize);
  };

  void mergeSubConfigs(const pugi::xml_node& aKeyNode, pugi::xml_node& aNewKeyNode,
      const std::string& aMainConfigPath) const;

  /**
   * Takes an XML document merges leafes from <table> & <state> under the same <key> and with the same ID.
   */
  void mergeTables(const pugi::xml_node& aKeyNode, pugi::xml_node& aNewKeyNode) const;

  /**
   * Normalises the path of subconfigs included with the <load> tag.
   * If the SubConfigPath is absolute, this function will not do anything.
   * If the path is relative it will be transformed into an absolute path
   * with repect to the MainConfigPath.
   */
  const std::string normaliseSubConfigPath(const std::string& aMainConfigPath, const std::string& aSubConfigPath) const;

};

DEFINE_SWATCH_EXCEPTION(InvalidConfig);
DEFINE_SWATCH_EXCEPTION(XmlFileError);

}
}

#endif	/* __SWATCH_XML_XMLINCLUDE_HPP__ */
