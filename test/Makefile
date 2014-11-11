SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/core/test
PackagePath = $(SWATCH_ROOT)/test
PackageName = cactuscore-swatch-test

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

# Library = cactus_swatch_test

Includes = 	\
		include/ \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/processor/include \
		${SWATCH_ROOT}/system/include \
		${SWATCH_ROOT}/system/test/include \
		${CACTUS_ROOT}/include

LibraryPaths =	\
		lib \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/core/test/lib \
		${SWATCH_ROOT}/processor/lib \
		${SWATCH_ROOT}/system/lib \
		${SWATCH_ROOT}/system/test/lib \
		${CACTUS_ROOT}/lib

Libraries =	\
		boost_system \
		boost_filesystem \
		boost_regex \
		boost_thread \
		boost_unit_test_framework \
		cactus_extern_pugixml \
		cactus_uhal_uhal \
		cactus_uhal_log \
		cactus_swatch_core \
		cactus_swatch_core_test \
		cactus_swatch_processor \
		cactus_swatch_system \
		cactus_swatch_system_test \

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
