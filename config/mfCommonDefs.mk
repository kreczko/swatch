ifndef SWATCH_ROOT
  SWATCH_ROOT:=${SWATCH_BACK_TO_ROOT}
endif

BUILD_HOME:=${SWATCH_ROOT}

$(info Using SWATCH_ROOT=${SWATCH_ROOT})

# Cactus config. This section shall be sources from /opt/cactus/config
CACTUS_ROOT ?= /opt/cactus
CACTUS_PLATFORM=$(shell /usr/bin/python -c "import platform; print platform.platform()")
CACTUS_OS="unknown.os"
# XDAQ config
XDAQ_ROOT ?= /opt/xdaq
DEBUG ?= 1

# OS Identification
UNAME=$(strip $(shell uname -s))
ifeq ($(UNAME),Linux)

ifneq ($(findstring redhat-5,$(CACTUS_PLATFORM)),)
CACTUS_OS=slc5

else ifneq ($(findstring redhat-6,$(CACTUS_PLATFORM)),)
CACTUS_OS=slc6

else ifneq ($(findstring centos-7,$(CACTUS_PLATFORM)),)
CACTUS_OS=cc7
endif

else ifeq ($(UNAME),Darwin)
CACTUS_OS=osx
endif

$(info OS Detected: $(CACTUS_OS))
# end of Cactus config

## Environment
# Make sure $CACTUS_ROOT/lib is present in LD_LIBRARY_PATH

ifeq ($(findstring $(CACTUS_ROOT)/lib,$(LD_LIBRARY_PATH)),)
$(info CACTUS_ROOT/lib added to LD_LIBRARY_PATH)
LD_LIBRARY_PATH:=$(CACTUS_ROOT)/lib:$(LD_LIBRARY_PATH)
else
$(info CACTUS_ROOT already in LD_LIBRARY_PATH)
endif

ifeq ($(findstring $(XDAQ_ROOT)/lib,$(LD_LIBRARY_PATH)),)
$(info XDAQ_ROOT/lib added to LD_LIBRARY_PATH)
LD_LIBRARY_PATH:=$(XDAQ_ROOT)/lib:$(LD_LIBRARY_PATH)
else
$(info XDAQ_ROOT already in LD_LIBRARY_PATH)
endif

export LD_LIBRARY_PATH

$(info Linker path: $(LD_LIBRARY_PATH))

# Compilers
CPP:=g++
LD:=g++
	
# Tools
MakeDir=mkdir -p

#python
PYTHON_VERSION ?= $(shell python -c "import distutils.sysconfig;print distutils.sysconfig.get_python_version()")
PYTHON_INCLUDE_PREFIX ?= $(shell python -c "import distutils.sysconfig;print distutils.sysconfig.get_python_inc()")

ifndef DEBUG
# Compiler flags
CxxFlags = -g -Wall -Werror=return-type -O3 -MMD -MP -fPIC -std=c++0x
LinkFlags = -g -shared -fPIC -Wall -O3 
ExecutableLinkFlags = -g -Wall -O3
else
CxxFlags = -g -ggdb -Wall -Werror=return-type -MMD -MP -fPIC -std=c++0x
LinkFlags = -g -ggdb -shared -fPIC -Wall
ExecutableLinkFlags = -g -ggdb -Wall
endif

