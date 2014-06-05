BUILD_HOME = $(shell pwd)/../../..

include $(BUILD_HOME)/config/Makefile.macros

Project = cactuscore
Package = swatch/system
PackagePath = $(CACTUS_RPM_ROOT)/$(Project)/${Package}
PackageName = cactuscore-swatch-system

Packager = Carlos Ghabrous Larrea

PACKAGE_VER_MAJOR = 0
PACKAGE_VER_MINOR = 0
PACKAGE_VER_PATCH = 0
PACKAGE_RELEASE = 0


LIBRARY = lib/libcactus_swatch_system.so


SOURCES = $(wildcard src/common/*.cpp)


OBJECT_FILES = $(patsubst src/common/%.cpp,obj/%.o,${SOURCES})


LIBRARY_PATH = \
				-L${BUILD_HOME}/${Project}/swatch/core/lib \
				-L${BUILD_HOME}/${Project}/swatch/board/lib \
				-L${CACTUS_ROOT}/lib

LIBRARIES = \
				-lcactus_swatch_core \
				-lcactus_swatch_board \
				-lcactus_uhal_uhal


INCLUDE_PATH = \
				-Iinclude  \
				-I${BUILD_HOME}/${Project}/swatch/core/include \
				-I${BUILD_HOME}/${Project}/swatch/board/include \
				-I${CACTUS_ROOT}/include


CPP_FLAGS = -g -O3 -Wall -MMD -MP -fPIC


LINK_FLAGS = -g -shared -fPIC -Wall -O3 ${LIBRARY_PATH} ${LIBRARIES}


RPMBUILD_DIR = ${PackagePath}/RPMBUILD

.PHONY: all _all clean _cleanall build _buildall install _installall rpm _rpmall test _testall

default: build
clean: _cleanall
_cleanall:
	rm -rf ${RPMBUILD_DIR}
	rm -rf obj
	rm -rf lib

all: _all
build: _all
buildall: _all
_all: ${LIBRARY}


${LIBRARY}: ${OBJECT_FILES}
	g++ ${LINK_FLAGS} ${OBJECT_FILES} -o $@

${OBJECT_FILES}: obj/%.o : src/common/%.cpp
	mkdir -p {lib,obj}
	g++ ${INCLUDE_PATH} ${CPP_FLAGS} -c $< -o $@

-include $(OBJECT_FILES:.o=.d)

rpm: _rpmall
_rpmall: 
	mkdir -p ${RPMBUILD_DIR}/{RPMS/{i386,i586,i686,x86_64},SPECS,BUILD,SOURCES,SRPMS}
	mkdir -p ${RPMBUILD_DIR}/SOURCES/{lib,include/swatch/system}
	cp -p lib/*.so ${RPMBUILD_DIR}/SOURCES/lib/.
	cp -p include/swatch/system/*.h ${RPMBUILD_DIR}/SOURCES/include/swatch/system/.
	rpmbuild -bb -bl --buildroot=${RPMBUILD_DIR}/BUILD						\
			--define  "_topdir ${RPMBUILD_DIR}"						\
			--define "_prefix ${CACTUS_ROOT}"							\
			--define "sources_dir ${RPMBUILD_DIR}/SOURCES"					\
			--define "name ${PackageName}"								\
			--define "version ${PACKAGE_VER_MAJOR}.${PACKAGE_VER_MINOR}.${PACKAGE_VER_PATCH}"	\
			--define "release ${PACKAGE_RELEASE}.${CACTUS_OS}"							\
			--define "packager ${Packager}"								\
			${PackageName}.spec

