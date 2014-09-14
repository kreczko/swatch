SWATCH_BACK_TO_ROOT= $(shell pwd)/..

include ${SWATCH_BACK_TO_ROOT}/config/mfCommonDefs.mk

CxxFlags += \
		-ftemplate-depth-128 \
		-O0 -rdynamic -finline-functions \
		-Wno-inline -DNDEBUG

Project = cactuscore
Package = swatch/pyswatch

PackagePath = $(SWATCH_ROOT)/pyswatch
PackageName = cactuscore-swatch-pyswatch
PythonModule = swatch

Packager = Alessandro Thea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 1
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

# The leading undescore is meant to be there
Library = cactus_swatch_pyswatch
LibraryTarget = lib/_pyswatch.so

Includes =	\
		include  \
		${SWATCH_ROOT}/core/include \
		${PYTHON_INCLUDE_PREFIX} \
		${CACTUS_ROOT}/include

LibraryPaths =  \
        lib \
        ${SWATCH_ROOT}/core/lib \
		${CACTUS_ROOT}/lib

Libraries =	\
		pthread \
		dl \
		util \
		\
		boost_filesystem \
		boost_python \
		boost_regex \
		boost_system \
		boost_thread \
		cactus_swatch_core

ExecutableLibraries = ${Libraries} python${PYTHON_VERSION}

include ${SWATCH_ROOT}/config/mfRules.mk
include ${SWATCH_ROOT}/config/mfPythonRPMRules.mk