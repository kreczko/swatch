SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/core

PackagePath = $(SWATCH_ROOT)/core
PackageName = cactuscore-swatch-core

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 4
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = cactus_swatch_core

Includes = \
	include \
	${SWATCH_ROOT}/logger/include \
	${CACTUS_ROOT}/include \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/linux

LibraryPaths = lib \
	$(XDAQ_ROOT)/lib \
	$(CACTUS_ROOT)/lib \
	/lib64

Libraries =	\
	log4cplus \
	uuid \
	config \
	xerces-c \
	toolbox \
	xcept \
	xdata \
	asyncresolv \
	mimetic \
	xoap \
	boost_regex \
	pthread \
	boost_thread \
        cactus_extern_pugixml
	

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
