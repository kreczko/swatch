if [[ $_ == $0 ]]; then  
  echo "This script is meant to be sourced:"
  echo "  source $0"
  exit 0
fi

#
function pathadd() {
  # TODO add check for empty path
  # and what happens if $1 == $2
  # Copy into temp variables
  PATH_NAME=$1
  PATH_VAL=${!1}
  if [[ ":$PATH_VAL:" != *":$2:"* ]]; then
    PATH_VAL="$2${PATH_VAL:+":$PATH_VAL"}"
    echo "- $1 += $2"

    # use eval to reset the target
    eval "$PATH_NAME=$PATH_VAL"
  fi
}

CACTUS_ROOT=${CACTUS_ROOT:-"/opt/cactus"}
XDAQ_ROOT=${XDAQ_ROOT:-"/opt/xdaq"}

pathadd LD_LIBRARY_PATH "${XDAQ_ROOT}/lib"
pathadd LD_LIBRARY_PATH "${CACTUS_ROOT}/lib"

SWATCH_TESTS=$( readlink -f $(dirname $BASH_SOURCE)/ )
SWATCH_ROOT=$( readlink -f ${SWATCH_TESTS}/.. )

pathadd PATH "${SWATCH_ROOT}/test/bin"

pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/logger/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/core/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/xml/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/processor/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/system/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/test/lib"

# tests
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/core/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/xml/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/processor/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/system/test/lib"

# include boost test framework's messages of form Entering/leaving case/suite "blah"
BOOST_TEST_LOG_LEVEL=test_suite

export PATH PYTHONPATH LD_LIBRARY_PATH CACTUS_ROOT SWATCH_ROOT SWATCH_TESTS BOOST_TEST_LOG_LEVEL
