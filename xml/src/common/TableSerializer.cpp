/* 
 * File:   TableSerializer.cpp
 * Author: ale
 * 
 * Created on March 3, 2016, 1:35 PM
 */

#include "swatch/xml/TableSerializer.hpp"
#include "xdata/Table.h"
#include "xdata/TableIterator.h"

// Boost Headers
#include <boost/tokenizer.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/range/adaptor/transformed.hpp>
//#include <boost/bind.hpp>

#include "swatch/xml/SimpleSerializer.hpp"

namespace swatch {
namespace xml {

const std::string TableSerializer::kColsTag = "columns";
const std::string TableSerializer::kTypesTag = "types";
const std::string TableSerializer::kRowsTag = "rows";
const std::string TableSerializer::kRowTag = "row";

TableSerializer::TableSerializer()
{
  	// register simple serliazers
	this->addDefault(new xdata::UnsignedInteger());
	this->addDefault(new xdata::Integer());
	this->addDefault(new xdata::Float());
	this->addDefault(new xdata::Boolean());
	this->addDefault(new xdata::String());
}


TableSerializer::~TableSerializer()
{
  std::map<std::string, xdata::Serializable*>::iterator it;
  for( it = mDefaults.begin(); it != mDefaults.end(); ++it)
    if (it->second) delete it->second;
}


void TableSerializer::addDefault(xdata::Serializable * aSerializable) 
{
    mDefaults[aSerializable->type()] = aSerializable;
}


std::vector<std::string> TableSerializer::splitAndTrim(const std::string& aStr, const std::string aDelimiter)
{
  // Instantiate the container for the column definition
  std::vector<std::string> lTokens;
  
  // Split the string according to the chosen delimiter
  boost::split(lTokens, aStr, boost::is_any_of(aDelimiter));
  
  // trim whitespaces
  std::for_each(
    lTokens.begin(),
    lTokens.end(),
    boost::bind(boost::algorithm::trim<std::string>, _1, std::locale())
  );
  return lTokens;
}

xdata::Serializable* TableSerializer::import(const pugi::xml_node& aNode)
{
  
  aNode.print(std::cout, "", pugi::format_raw);std::cout << std::endl;

  // Get the delimiter
  pugi::xml_attribute lDelimiterAttr(aNode.attribute("delimiter"));
  std::string lDelimiter(lDelimiterAttr.empty() ? "," : lDelimiterAttr.value());
  // And the main nodes of the table
  pugi::xml_node lColsNode = aNode.child(kColsTag.c_str());
  pugi::xml_node lTypesNode = aNode.child(kTypesTag.c_str());
  pugi::xml_node lRows = aNode.child(kRowsTag.c_str());

  if ( !( lColsNode && lTypesNode && lRows) ) {
    std::ostringstream msg;
    msg << "Missing nodes: ";
    if ( !lColsNode ) msg << "'" << kColsTag << "' ";
    if ( !lTypesNode ) msg << "'" << kTypesTag << "' ";
    if ( !lRows ) msg << "'" << kRowsTag << "' ";
    
    throw ValueError(msg.str());
  }
  
  std::vector<std::string> lColNames = splitAndTrim(lColsNode.child_value(), lDelimiter);
  std::vector<std::string> lTypeNames = splitAndTrim(lTypesNode.child_value(), lDelimiter);

  
  if ( lColNames.size() != lTypeNames.size()) {
    std::cout << lColNames.size() << " - " << lTypeNames.size() << std::endl;
    throw ValueError("Vaccaboia");
  }
  
  xdata::Table* lTable = new xdata::Table();

  std::vector<xdata::Serializable*> lColumnDefaults(lColNames.size());
  std::string lNormalizedType;
  for ( size_t c(0); c<lColNames.size(); ++c ) {
    lNormalizedType = normaliseType(lTypeNames[c]);
    lTable->addColumn(lColNames[c], lNormalizedType);
    lColumnDefaults[c] = mDefaults[lNormalizedType];
  }
  
  
  std::string lRowStr;
  std::vector<std::string> lElements(lColNames.size());

  for (pugi::xml_node lKey = lRows.first_child(); lKey; lKey = lKey.next_sibling()) {
    if ( lKey.name() != kRowTag ) {
      delete lTable;
      throw ValueError("Pizello");
    }
    std::cout << "row!" << std::endl;

    lElements = splitAndTrim(lKey.child_value(),lDelimiter);

    if ( lColNames.size() != lElements.size()) {
      delete lTable;
      throw ValueError("Cacca");
    }
    

    xdata::Table::Row& lRow = *lTable->append();

    for ( size_t k(0); k<lColNames.size(); ++k) {
      lRow.setField(lColNames[k],*lColumnDefaults[k]);
      lRow.getField(lColNames[k])->fromString(lElements[k]);
    }
    

  }

  return lTable;
}


const xdata::Serializable* TableSerializer::const_import(const pugi::xml_node& aNode){
    return const_cast<xdata::Serializable*>(import(aNode));
}


std::string TableSerializer::type() const
{
  xdata::Table tmp;
  return tmp.type();
}

// Copied from XmlSerializer
std::string TableSerializer::normaliseType(const std::string& aType) {
	std::string lType(aType);
	// expand type names
	if (lType == "vector:uint")
		lType = "vector:unsigned int";
	if (lType == "uint")
		lType = "unsigned int";

	return lType;
}

} // namespace xml
} // namespace swatch
