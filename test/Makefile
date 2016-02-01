SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/core/test
PackagePath = $(SWATCH_ROOT)/test
PackageName = cactuscore-swatch-test

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 4
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

# Library = cactus_swatch_test

Includes = 	\
		include/ \
		${SWATCH_ROOT}/logger/include \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/processor/include \
		${SWATCH_ROOT}/processor/test/include \
		${SWATCH_ROOT}/system/include \
		${SWATCH_ROOT}/system/test/include \
		${SWATCH_ROOT}/xml/include \
		${SWATCH_ROOT}/xml/test/include \
		${CACTUS_ROOT}/include \
		$(XDAQ_ROOT)/include \
		$(XDAQ_ROOT)/include/linux

LibraryPaths =	\
		lib \
		${SWATCH_ROOT}/logger/lib \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/core/test/lib \
		${SWATCH_ROOT}/processor/lib \
		${SWATCH_ROOT}/processor/test/lib \
		${SWATCH_ROOT}/system/lib \
		${SWATCH_ROOT}/system/test/lib \
		${SWATCH_ROOT}/xml/lib \
		${SWATCH_ROOT}/xml/test/lib \
		${CACTUS_ROOT}/lib \
		$(XDAQ_ROOT)/lib \

Libraries =	\
		boost_system \
		boost_filesystem \
		boost_regex \
		boost_thread \
		boost_unit_test_framework \
		cactus_uhal_uhal \
		cactus_uhal_log \
		cactus_swatch_logger \
		cactus_swatch_core \
		cactus_swatch_core_test \
		cactus_swatch_processor \
		cactus_swatch_processor_test \
		cactus_swatch_system \
		cactus_swatch_system_test \
		cactus_swatch_xml \
		cactus_swatch_xml_test \
		dl

ExecutableLibraries = ${Libraries} ${Library}

IgnoreSources = src/common/console_test_runner.cxx
# Temporary, horrible hack
# console_test_runner must not be linked to cactus_core_test nor to the other test libraries
# and therefore needs a dedicated build target
ExtraTargets += bin/console_test_runner.exe
	
include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk

# Extra target after all others
bin/console_test_runner.exe: obj/console_test_runner.o
	${LD} ${CxxFlags} -L${CACTUS_ROOT}/lib -lboost_unit_test_framework -ldl $< -o $@
