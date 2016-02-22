SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/mp7

PackagePath = $(SWATCH_ROOT)/mp7
PackageName = cactuscore-swatch-mp7

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 5
PACKAGE_VER_PATCH = 1
PACKAGE_RELEASE = 0

Library = cactus_swatch_mp7

Includes = \
	include \
	${SWATCH_ROOT}/logger/include \
	${SWATCH_ROOT}/core/include \
	${SWATCH_ROOT}/processor/include \
	${CACTUS_ROOT}/include \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/linux
	
LibraryPaths = \
	lib \
	${SWATCH_ROOT}/logger/lib \
	${SWATCH_ROOT}/core/lib \
	${SWATCH_ROOT}/processor/lib \
	${CACTUS_ROOT}/lib \
	$(XDAQ_ROOT)/lib

Libraries = \
	cactus_uhal_uhal \
	cactus_mp7_mp7 \
	cactus_swatch_logger \
	cactus_swatch_core \
	cactus_swatch_processor 

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
