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

/**
 * Same as xmlFileToSystemStub, but reads from a pugi::xml_document instead of a file.
 */
swatch::system::SystemStub xmlToSystemStub(const pugi::xml_document& aXmlDoc);

DEFINE_SWATCH_EXCEPTION(InvalidSystemDescription);

namespace detail {

/**
 * Converts a <daqttc-mgr> tag to swatch::dtm::DaqTTCStub
 */
swatch::dtm::DaqTTCStub xmlToDaqTTCStub(const pugi::xml_node& aNode);

/**
 * Converts a <processor> tag to swatch::processor::ProcessorStub
 */
swatch::processor::ProcessorStub xmlToProcessorStub(const pugi::xml_node& aNode);

/**
 * Converts the <tx-port> and <rx-port> subtags of <processor> into a vector of swatch::processor::ProcessorPortStub
 */
std::vector<swatch::processor::ProcessorPortStub> xmlToProcessorPortStub(const pugi::xml_node& aNode);

/**
 * Converts a <crate> tag to swatch::system::CrateStub
 */
swatch::system::CrateStub xmlToCrateStub(const pugi::xml_node& aNode);

/**
 * Converts a <link> tag to a vector of swatch::system::LinkStub
 */
std::vector<swatch::system::LinkStub> xmlToLinkStubs(const pugi::xml_node& aNode);

/**
 * Converts a <connected-fed> tag to swatch::system::SystemStub::FEDInputPortsMap
 */
swatch::system::SystemStub::FEDInputPortsMap xmlToConnectedFeds(const pugi::xml_node& aNode);

/**
 * Validates the swatch::system::System XML description from an XML document
 * <system>
 *   ...
 * </system>
 */
bool validateSystemXml(const pugi::xml_document& aXmlDoc);

/**
 * Validates a single instance of a <system> child tag (e.g <crate>, <creator>, etc)
 */
bool validateChildXml(const pugi::xml_node& aChildNode);

/**
 * Validates the <creator> tag
 * e.g
 * <creator>calol2::MainProcessor</creator>
 */
bool validateCreatorXml(const pugi::xml_node& aNode);

/**
 * Validates the <crate> tag
 * <crate id="...">
 *  <location>...</location>
 *  <description>...</description>
 * </crate>
 */
bool validateCrateXml(const pugi::xml_node& aNode);

/**
 * Validates the <processor> tag
 * <processor id="...">
 *   <creator>...</creator>
 *   <hw-type>...</hw-type>
 *   <role>...</role>
 *   <uri>...</uri>
 *   <address-table>...</address-table>
 *   <crate>...</crate>
 *   <slot>...</slot>
 *   <rx-port name="..." pid="..."/>
 *   <tx-port name="..." pid="..."/>
 * </processor>
 */
bool validateProcessorXml(const pugi::xml_node& aNode);

/**
 * Validates the <daqttc-mgr> tag
 * <daqttc-mgr id="...">
 *   <creator>...</creator>
 *   <hw-type>...</hw-type>
 *   <role>...</role>
 *   <uri>...</uri>
 *   <uri>...</uri>
 *   <address-table>...</address-table>
 *   <address-table>...</address-table>
 *   <crate>...</crate>
 *   <slot>...</slot>
 *   <fed-id>...</fed-id>
 * </daqttc-mgr>
 */
bool validateDAQTTCXml(const pugi::xml_node& aNode);

/**
 * Validates a single instance of <link>:
 * <link id="...">
 *   <from>...</from>
 *   <to>...</to>
 *   <tx-port>...</tx-port>
 *   <rz-port>...</rx-port>
 * </link>
 */
bool validateLinkXml(const pugi::xml_node& aNode);

/**
 * Validates a single instance of <connected-feds>:
 * <connected-fed id="...">
 *   <port id="...]"/>
 *   <port id="..."/>
 * </connected-fed>
 * </link>
 */
bool validateConnectedFedXml(const pugi::xml_node& aNode);
/**
 * Checks if the content of a child node, <child> is not empty
 * <aNode>
 *   <aChildName>CONTENT</aChildName>
 * </aNode>
 */
bool childContentNonEmpty(const pugi::xml_node& aNode, const std::string& aChildName);

/**
 * Checks if aNode has a non-empty attribute of name aAttr
 * <aNode aAttr="..." />
 */
bool hasAttr(const pugi::xml_node& aNode, const std::string& aAttr);

/**
 * Returns the number of children of aNode
 * <aNode>
 *   <child1 />
 *   <child2 />
 *   ...
 * </aNode>
 */
size_t numberOfChildren(const pugi::xml_node& aNode);

/**
 * Returns the number of children of aNode with specific name
 * <aNode>
 *   <aChildName /> <!-- counted -->
 *   <aChildName /> <!-- counted -->
 *   <child2 /> <!-- not counted -->
 *   ...
 * </aNode>
 */
size_t numberOfChildren(const pugi::xml_node& aNode, const std::string& aChildName);

/**
 * Loads an XML file, aFilePath, into a pugi::xml_document, aDocToLoadInto
 */
void loadFromFile(const std::string& aFilePath, pugi::xml_document& aDocToLoadInto);

void pushBackPortStubs(std::vector<swatch::processor::ProcessorPortStub>& aPortStubs, const std::string& aName,
    const std::string& aIndex);

} //ns: detail
} //ns: system
} //ns: xml
} //ns: swatch

#endif  /* __SWATCH_XML_XMLSYSTEM_HPP__ */
