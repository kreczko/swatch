SWATCH_BACK_TO_ROOT= $(shell pwd)/../..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/core/test
PackagePath = $(SWATCH_ROOT)/core/test
PackageName = cactuscore-swatch-core-test

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = cactus_swatch_core_test

Includes = 	\
		include \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/logger/include \
		${CACTUS_ROOT}/include \
	    $(XDAQ_ROOT)/include \
	    $(XDAQ_ROOT)/include/linux

LibraryPaths =	\
		lib \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/logger/lib \
		${CACTUS_ROOT}/lib \
		$(XDAQ_ROOT)/lib

Libraries =	\
		boost_system \
		boost_unit_test_framework \
		cactus_swatch_core \
		cactus_swatch_logger \
		toolbox

ExecutableLibraries = ${Libraries} ${Library}

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk