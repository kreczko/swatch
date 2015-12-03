// Standard headers

// boost headers

// SWATCH headers
#include "swatch/xml/XmlSerializer.hpp"
#include "swatch/xml/SingleObjectSerializer.hpp"
#include "swatch/xml/VectorSerializer.hpp"

namespace swatch {
namespace xml {

XmlSerializer::XmlSerializer() {
	// simple serliazers
	this->addObjectSerializer(new SingleObjectSerializer<xdata::UnsignedInteger>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Integer>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Float>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::Boolean>());
	this->addObjectSerializer(new SingleObjectSerializer<xdata::String>());
	// vector serializers
	this->addObjectSerializer(new VectorSerializer<xdata::UnsignedInteger>());
	this->addObjectSerializer(new VectorSerializer<xdata::Integer>());
	this->addObjectSerializer(new VectorSerializer<xdata::Float>());
	this->addObjectSerializer(new VectorSerializer<xdata::Boolean>());
	this->addObjectSerializer(new VectorSerializer<xdata::String>());
}

XmlSerializer::~XmlSerializer() {

}

xdata::Serializable* XmlSerializer::import(const pugi::xml_node& aNode) {
	std::string lType(aNode.attribute("type").value());
	xdata::Serializable* lSerializable(0x0);
	ObjectSerializer* lSerializer(0x0);

	if (lType.find("vector:") != std::string::npos) {
		//use vector serializer
		if (lType == "vector:uint")
			lType = "vector:unsigned int";
		if (lType == "vector:int")
			lType = "vector:integer";
		lSerializer = dynamic_cast<ObjectSerializer*>(this->getObjectSerializer(lType));
	} else if (lType.find("table:") != std::string::npos) {
		//use table serializer
	} else {
		if (lType == "uint")
			lType = "unsigned int";
		if (lType == "int")
			lType = "integer";
		lSerializer = dynamic_cast<ObjectSerializer*>(this->getObjectSerializer(lType));
	}
	lSerializable = lSerializer->import(aNode);

	return lSerializable;
}

}
}
