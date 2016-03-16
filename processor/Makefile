SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/processor

PackagePath = $(SWATCH_ROOT)/processor
PackageName = cactuscore-swatch-processor

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 7
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0.alpha0

Library = cactus_swatch_processor


Includes =	\
		include  \
		${SWATCH_ROOT}/core/include \
		${CACTUS_ROOT}/include \
		$(XDAQ_ROOT)/include \
		$(XDAQ_ROOT)/include/linux

LibraryPaths =  \
        lib \
        ${SWATCH_ROOT}/core/lib \
		${CACTUS_ROOT}/lib \
		$(XDAQ_ROOT)/lib \ 
	
Libraries =	\
		cactus_swatch_core

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
