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
SWATCH_TESTS=$( readlink -f $(dirname $BASH_SOURCE)/ )
SWATCH_ROOT=$( readlink -f ${SWATCH_TESTS}/.. )

pathadd PYTHONPATH "${SWATCH_ROOT}/pyswatch/pkg"
pathadd PYTHONPATH "${SWATCH_ROOT}/pyswatch/lib"

pathadd PATH "${SWATCH_ROOT}/test/bin"

pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/core/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/processor/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/system/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/database/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/hardware/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/test/lib"

# tests
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/core/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/processor/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/database/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/system/test/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/hardware/test/lib"

export PATH PYTHONPATH LD_LIBRARY_PATH CACTUS_ROOT SWATCH_ROOT SWATCH_TESTS 
