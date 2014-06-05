BUILD_HOME = $(shell pwd)/../../..

include $(BUILD_HOME)/config/Makefile.macros

Project = cactuscore
Package = swatch/test
PackagePath = $(CACTUS_RPM_ROOT)/$(Project)/${Package}
PackageName = cactuscore-swatch-test

Packager = Carlos Ghabrous Larrea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 0
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0

LIBRARY = 
LIBRARY_SOURCES = 
LIBRARY_OBJECT_FILES = 

EXECUTABLE_SOURCES = $(wildcard src/common/*.cxx)
EXECUTABLE_OBJECT_FILES = $(patsubst src/common/%.cxx,obj/%.o,${EXECUTABLE_SOURCES})
EXECUTABLES = $(patsubst src/common/%.cxx,bin/%.exe,${EXECUTABLE_SOURCES})

$(info ${EXECUTABLE_SOURCES})
$(info ${EXECUTABLE_OBJECT_FILES})
$(info ${EXECUTABLE})



LIBRARY_PATH = \
			-L${BUILD_HOME}/${Project}/swatch/core/lib \
			-L${BUILD_HOME}/${Project}/swatch/board/lib \
			-L${BUILD_HOME}/${Project}/swatch/system/lib \
			-L${CACTUS_ROOT}/lib

LIBRARIES = 	\
			-lboost_system \
			-lboost_filesystem \
			-lboost_regex \
			-lboost_thread \
			-lcactus_extern_pugixml \
			-lcactus_uhal_grammars \
			-lcactus_uhal_log \
			-lcactus_uhal_uhal \
			-lcactus_swatch_core \
			-lcactus_swatch_board \
			-lcactus_swatch_system 

EXECUTABLE_LIBRARIES = ${LIBRARIES}

INCLUDE_PATH = 	\
				-I${BUILD_HOME}/${Project}/swatch/core/include \
				-I${BUILD_HOME}/${Project}/swatch/board/include \
				-I${BUILD_HOME}/${Project}/swatch/system/include \
				-I${CACTUS_ROOT}/include

CPP_FLAGS = -g -Wall -O3 -MMD -MP -fPIC 

LINK_LIBRARY_FLAGS = 

LINK_EXECUTABLE_FLAGS = -g -Wall -O3 ${LIBRARY_PATH} ${LIBRARIES}

RPMBUILD_DIR = ${PackagePath}/RPMBUILD

.PHONY: all _all clean _cleanall build _buildall install _installall rpm _rpmall test _testall spec_update

default: build

clean: _cleanall
_cleanall:
	rm -rf ${RPMBUILD_DIR}
	rm -rf obj
	rm -rf bin
	rm -rf lib

all: _all
build: _all
buildall: _all
_all: ${EXECUTABLES}


${EXECUTABLES}: bin/%.exe: obj/%.o ${EXECUTABLE_OBJECT_FILES}
	g++ ${LINK_EXECUTABLE_FLAGS} $< -o $@

${EXECUTABLE_OBJECT_FILES}: obj/%.o : src/common/%.cxx 
	mkdir -p {bin,obj,lib}
	g++ -c ${CPP_FLAGS} ${INCLUDE_PATH} $< -o $@

-include $(EXECUTABLE_OBJECT_FILES:.o=.d)


rpm: _rpmall
_rpmall: 
	mkdir -p ${RPMBUILD_DIR}/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
	mkdir -p ${RPMBUILD_DIR}/SOURCES/{bin/${Package},lib,include/${Package},etc/${Package}}
	cp -p include/${Package}/*.hpp ${RPMBUILD_DIR}/SOURCES/include/${Package}/.
	cp -p bin/*.exe ${RPMBUILD_DIR}/SOURCES/bin/${Package}/.
	cp -p src/python/test_pycohal ${RPMBUILD_DIR}/SOURCES/bin/${Package}/.
	cp -p scripts/* ${RPMBUILD_DIR}/SOURCES/bin/${Package}/.
	cp -p lib/*.so ${RPMBUILD_DIR}/SOURCES/lib/.
	cp -p etc/${Package}/*.xml ${RPMBUILD_DIR}/SOURCES/etc/${Package}
	rpmbuild -bb -bl --buildroot=${RPMBUILD_DIR}/BUILD						\
			--define  "_topdir ${RPMBUILD_DIR}"						\
			--define "_prefix ${CACTUS_ROOT}"							\
			--define "sources_dir ${RPMBUILD_DIR}/SOURCES"					\
			--define "name ${PackageName}"								\
			--define "version ${PACKAGE_VER_MAJOR}.${PACKAGE_VER_MINOR}.${PACKAGE_VER_PATCH}"	\
			--define "release ${PACKAGE_RELEASE}.${CACTUS_OS}"							\
			--define "packager ${Packager}"								\
			${PackageName}.spec

