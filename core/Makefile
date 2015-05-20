SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

Project = cactuscore
Package = swatch/core

PackagePath = $(SWATCH_ROOT)/core
PackageName = cactuscore-swatch-core

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
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
	toolbox \
	xcept \
	log4cplus \
	config \
	asyncresolv \
	xdaq \
	xdata \
	xerces-c \
	xoap \
	mimetic \
	cgicc \
	xgi\
	uuid \
	logxmlappender \
	logudpappender \
	peer \
	boost_regex
#	tcpla \
	

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfRPMRules.mk
