// Standard headers

// boost headers

// SWATCH headers
#include "swatch/xml/XmlSerializer.hpp"
#include "swatch/xml/SingleObjectSerializer.hpp"
#include "swatch/xml/VectorSerializer.hpp"
//log4cplus headers
#include <log4cplus/loggingmacros.h>

namespace swatch {
namespace xml {

XmlSerializer::XmlSerializer() :
	mLogger(swatch::logger::Logger::getInstance("swatch.xml.XmlSerializer")){
	// register simple serliazers
	this->addObjectSerializer(new SingleObjectSerializer<xdata::UnsignedInteger>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Integer>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Float>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Boolean>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::String>());
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
	ObjectSerializer* lSerializer(dynamic_cast<ObjectSerializer*>(this->getObjectSerializer(lType)));
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
