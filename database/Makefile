SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/database

PackagePath = $(SWATCH_ROOT)/database
PackageName = cactuscore-swatch-database

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = cactus_swatch_database

Includes = \
	include \
	${CACTUS_ROOT}/include \
	${SWATCH_ROOT}/core/include \
	
LibraryPaths = lib

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
