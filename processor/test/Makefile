SWATCH_BACK_TO_ROOT= $(shell pwd)/../..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/processor/test
PackagePath = $(SWATCH_ROOT)/processor/test
PackageName = cactuscore-swatch-processor-test

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 7
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 1

Library = cactus_swatch_processor_test

Includes = 	\
	include \
	${SWATCH_ROOT}/logger/include \
	${SWATCH_ROOT}/core/include \
	${SWATCH_ROOT}/core/test/include \
	${SWATCH_ROOT}/processor/include \
	${CACTUS_ROOT}/include \
	${XDAQ_ROOT}/include \
	${XDAQ_ROOT}/include \
	$(XDAQ_ROOT)/include/linux
		

LibraryPaths =	\
	lib \
	${SWATCH_ROOT}/logger/lib \
	${SWATCH_ROOT}/core/lib \
	${SWATCH_ROOT}/core/test/lib \
	${SWATCH_ROOT}/processor/lib \
	${CACTUS_ROOT}/lib \
	${XDAQ_ROOT}/lib

Libraries =	\
	boost_system \
	boost_unit_test_framework \
	cactus_uhal_uhal \
	cactus_uhal_tests \
	cactus_swatch_logger \
	cactus_swatch_core \
	cactus_swatch_core_test \
	cactus_swatch_processor \
	dl


ExecutableLibraries = ${Libraries} ${Library}

#IgnoreSources = \
	src/common/testConfigure.cxx


include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
