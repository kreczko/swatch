/* 
 * File:   TableSerializer.hpp
 * Author: ale
 *
 * Created on March 3, 2016, 1:35 PM
 */

#ifndef __SWATCH_XML_TABLESERIALIZER_HPP__
#define	__SWATCH_XML_TABLESERIALIZER_HPP__

#include "swatch/xml/AbstractSerializer.hpp"
#include "xdata/Serializable.h"

#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {

class TableSerializer : public AbstractSerializer {
public:
  TableSerializer();
  virtual ~TableSerializer();
  /**
   * Converts an XML element into a xdata::Serializable
   */
  xdata::Serializable* import(const pugi::xml_node& aNode);
  const xdata::Serializable* const_import(const pugi::xml_node& aNode);

  /**
   * Implementation of xdata::ObjectSerializer::type;
   */
  std::string type() const;

private:
  static const std::string kColsTag;
  static const std::string kTypesTag;
  static const std::string kRowsTag;
  static const std::string kRowTag;
  
  //FIXME: copied from XmlSerializer
  static std::string normaliseType(const std::string& aType);
  
  void addDefault(xdata::Serializable * serializer);

  std::vector<std::string> splitAndTrim( const std::string& aStr, const std::string aDelimiter );
  
  // Dynamic binder to object handlers by type
  std::map<std::string, xdata::Serializable*> mDefaults;
};

} // namespace xml
} // namespace swatch

#endif	/* __SWATCH_XML_TABLESERIALIZER_HPP__ */

