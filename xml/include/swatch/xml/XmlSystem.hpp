#ifndef __SWATCH_XML_XMLSYSTEM_HPP__
#define __SWATCH_XML_XMLSYSTEM_HPP__

// Standard headers
#include <string>

// SWATCH headers
#include "swatch/core/exception.hpp"
#include "swatch/processor/ProcessorStub.hpp"
#include "swatch/system/LinkStub.hpp"
#include "swatch/dtm/DaqTTCStub.hpp"
#include "swatch/system/SystemStub.hpp"

// external
#include "pugixml/pugixml.hpp"

namespace swatch {
namespace xml {
namespace system {
extern const char* MAINTAG;
extern const std::vector<std::string> SUBTAGS;

/**
 * The xmlFileToSystemStub is a function to read, verify and transform XML files
 * into a swatch::system::SystemStub
 * The system XML is expected to have the form
 * <system id="calol2" creator="calol2::System">
 *  <crate id="S2D11-29" ... />
 *  <processor id="MP8" ... />
 *  <daqttc-mgr id="AMC13" .. />
 *    ...
 * </system>
 *
 * For a full example please see swatch/xml/test/etc/swatch/test/system.xml
 */
swatch::system::SystemStub xmlFileToSystemStub(const std::string& aFileName);

// TODO: move this to namespace detail?
swatch::system::SystemStub xmlToSystemStub(const pugi::xml_document& aXmlDoc);

swatch::dtm::DaqTTCStub xmlToDaqTTCStub(const pugi::xml_node& aNode);
swatch::processor::ProcessorStub xmlToProcessorStub(const pugi::xml_node& aNode);
swatch::processor::ProcessorStub xmlToProcessorPortStub(const pugi::xml_node& aNode);
swatch::system::CrateStub xmlToCrateStub(const pugi::xml_node& aNode);
std::vector<swatch::system::LinkStub> xmlToLinkStubs(const pugi::xml_node& aNode);

bool validateSystemXml(const pugi::xml_document& aXmlDoc);

bool hasAttr(const pugi::xml_node& aNode, const std::string& aAttr);

size_t numberOfChildren(const pugi::xml_node& aNode);
size_t numberOfChildren(const pugi::xml_node& aNode, const std::string& aChildName);

void loadFromFile(const std::string& aFilePath, pugi::xml_document& aDocToLoadInto);

DEFINE_SWATCH_EXCEPTION(InvalidSystemDescription);

} //ns: system
} //ns: xml
} //ns: swatch

#endif  /* __SWATCH_XML_XMLSYSTEM_HPP__ */
