SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/xml

PackagePath = $(SWATCH_ROOT)/xml
PackageName = cactuscore-swatch-xml

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 7
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 1

Library = cactus_swatch_xml


Includes =	\
		include  \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/processor/include \
		${SWATCH_ROOT}/system/include \
		${SWATCH_ROOT}/logger/include \
		${CACTUS_ROOT}/include \
		$(XDAQ_ROOT)/include \
		$(XDAQ_ROOT)/include/linux

LibraryPaths =  \
		lib \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/processor/lib \
		${SWATCH_ROOT}/system/lib \
		${SWATCH_ROOT}/logger/lib \
		${CACTUS_ROOT}/lib \
		$(XDAQ_ROOT)/lib
	
Libraries =	\
		cactus_swatch_core \
		cactus_swatch_logger \
		cactus_swatch_processor \
		cactus_swatch_system \
		cactus_extern_pugixml

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
