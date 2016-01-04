#!/bin/bash

SWATCH_DEV_TOOLS=$( readlink -f $(dirname $BASH_SOURCE)/ )
SWATCH_ROOT=${SWATCH_DEV_TOOLS}/..

INCLUDE_PATHS=-I/opt/cactus/include \
   -I/opt/xdaq/include \
   -I/opt/xdaq/include/linux \
   -I${SWATCH_ROOT}/logger/include \
   -I${SWATCH_ROOT}/core/include \
   -I${SWATCH_ROOT}/processor/include \
   -I${SWATCH_ROOT}/system/include \


cppcheck ${INCLUDE_PATHS} --dump {logger,core,processor,system}/src/common/*.cpp


