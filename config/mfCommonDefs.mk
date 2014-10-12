ifndef SWATCH_ROOT
  SWATCH_ROOT:=${SWATCH_BACK_TO_ROOT}
endif

BUILD_HOME:=${SWATCH_ROOT}

$(info Using SWATCH_ROOT=${SWATCH_ROOT})

# Cactus config. This section shall be sources from /opt/cactus/config
CACTUS_ROOT ?= /opt/cactus
CACTUS_PLATFORM=$(shell /usr/bin/python -c "import platform; print platform.platform()")
CACTUS_OS="unknown.os"

UNAME=$(strip $(shell uname -s))
ifeq ($(UNAME),Linux)
	ifneq ($(findstring redhat-5,$(CACTUS_PLATFORM)),)
	        CACTUS_OS=slc5
	else ifneq ($(findstring redhat-6,$(CACTUS_PLATFORM)),)
	        CACTUS_OS=slc6
	else ifneq ($(findstring centos-7,$(CACTUS_PLATFORM)),)
			CACTUS_OS=cc7
	endif
endif
ifeq ($(UNAME),Darwin)
    CACTUS_OS=osx
endif

$(info OS Detected: $(CACTUS_OS))
# end of Cactus config


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
CxxFlags = -g -Wall -O3 -MMD -MP -fPIC -std=c++0x
LinkFlags = -g -shared -fPIC -Wall -O3 
ExecutableLinkFlags = -g -Wall -O3
else
CxxFlags = -g -ggdb -Wall -MMD -MP -fPIC -std=c++0x
LinkFlags = -g -ggdb -shared -fPIC -Wall
ExecutableLinkFlags = -g -ggdb -Wall
endif

