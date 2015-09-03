SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/amc13

PackagePath = $(SWATCH_ROOT)/amc13
PackageName = cactuscore-swatch-amc13

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 2
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = cactus_swatch_amc13

Includes = \
	include \
	${SWATCH_ROOT}/logger/include \
	${SWATCH_ROOT}/core/include \
	${SWATCH_ROOT}/processor/include \
	${SWATCH_ROOT}/system/include \
	${CACTUS_ROOT}/include \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/linux
	
LibraryPaths = \
	lib \
	${SWATCH_ROOT}/logger/lib \
	${SWATCH_ROOT}/core/lib \
	${SWATCH_ROOT}/processor/lib \
	${SWATCH_ROOT}/system/lib \
	${CACTUS_ROOT}/lib \
	$(XDAQ_ROOT)/lib

Libraries = \
	cactus_uhal_uhal \
	cactus_amc13_amc13 \
	cactus_swatch_logger \
	cactus_swatch_core \
	cactus_swatch_processor \
	cactus_swatch_system

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
