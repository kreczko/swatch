SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuslogger
Package = swatch/logger

PackagePath = $(SWATCH_ROOT)/logger
PackageName = cactuscore-swatch-logger

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 7
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 1

Library = cactus_swatch_logger

Includes = \
	include \
	${CACTUS_ROOT}/include \
	${XDAQ_ROOT}/include
	
LibraryPaths = \
	lib \
	${XDAQ_ROOT}/lib

Libraries = log4cplus

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
