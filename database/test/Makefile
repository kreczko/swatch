SWATCH_BACK_TO_ROOT= $(shell pwd)/../..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/database/test
PackagePath = $(SWATCH_ROOT)/$(Project)/${Package}
PackageName = cactuscore-swatch-database-test

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = libcactus_swatch_database_test.so

Includes = 	\
		include \
		${SWATCH_ROOT}/database/include \
		

LibraryPaths =	\
		lib \
		${SWATCH_ROOT}/database/lib \
	

Libraries =	\
		boost_system \
		boost_unit_test_framework \
		cactus_swatch_database \


ExecutableLibraries = ${Libraries} cactus_swatch_database_test

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk