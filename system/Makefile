SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/system

PackagePath = $(SWATCH_ROOT)/system
PackageName = cactuscore-swatch-system

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

Library = cactus_swatch_system

Includes =	\
		include  \
		${SWATCH_ROOT}/core/include \
		${SWATCH_ROOT}/processor/include \
		${CACTUS_ROOT}/include

LibraryPaths = \
		${SWATCH_ROOT}/core/lib \
		${SWATCH_ROOT}/processor/lib \
		${CACTUS_ROOT}/lib

Libraries =	\
		cactus_swatch_core \
		cactus_swatch_processor \

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk