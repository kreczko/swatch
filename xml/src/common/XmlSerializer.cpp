// Standard headers

// boost headers

// SWATCH headers
#include "swatch/xml/XmlSerializer.hpp"
#include "swatch/xml/VectorSerializer.hpp"
#include "swatch/xml/SimpleSerializer.hpp"
//log4cplus headers
#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace xml {

XmlSerializer::XmlSerializer() :
	mLogger(swatch::logger::Logger::getInstance("swatch.xml.XmlSerializer")){
	// register simple serliazers
	this->addObjectSerializer(new SimpleSerializer<xdata::UnsignedInteger>());
	this->addObjectSerializer(new SimpleSerializer<xdata::Integer>());
	this->addObjectSerializer(new SimpleSerializer<xdata::Float>());
	this->addObjectSerializer(new SimpleSerializer<xdata::Boolean>());
	this->addObjectSerializer(new SimpleSerializer<xdata::String>());
	// register vector serializers
	this->addObjectSerializer(new VectorSerializer<xdata::UnsignedInteger>());
	this->addObjectSerializer(new VectorSerializer<xdata::Integer>());
	this->addObjectSerializer(new VectorSerializer<xdata::Float>());
	this->addObjectSerializer(new VectorSerializer<xdata::Boolean>());
	this->addObjectSerializer(new VectorSerializer<xdata::String>());
}

XmlSerializer::~XmlSerializer() {

}

xdata::Serializable* XmlSerializer::import(const pugi::xml_node& aNode) {
	LOG4CPLUS_DEBUG(mLogger, "Calling XmlSerializer::import");
	std::string lType(aNode.attribute("type").value());
	LOG4CPLUS_DEBUG(mLogger, "Normalising type: " + lType);
	lType = normaliseType(lType);
	LOG4CPLUS_DEBUG(mLogger, "Getting serializer for type " + lType);
	AbstractSerializer* lSerializer(0x0);
	try {
		lSerializer = dynamic_cast<AbstractSerializer*>(this->getObjectSerializer(lType));
	} catch (const xdata::exception::Exception& e){
		throw UnknownDataType("No serializer for type '" + lType + "' found.");
	}
	LOG4CPLUS_DEBUG(mLogger, "Found serializer with type " + lSerializer->type());
	xdata::Serializable* lSerializable(lSerializer->import(aNode));

	return lSerializable;
}

const std::string XmlSerializer::normaliseType(const std::string& aType) const {
	std::string lType(aType);
	// expand type names
	if (lType == "vector:uint")
		lType = "vector:unsigned int";
	if (lType == "uint")
		lType = "unsigned int";

	return lType;
}

}
}
