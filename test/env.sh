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

CACTUS_ROOT="/opt/cactus"
SWATCH_TESTS=$( readlink -f $(dirname $BASH_SOURCE)/ )
SWATCH_ROOT=$( readlink -f ${SWATCH_TESTS}/.. )

pathadd PATH "${SWATCH_ROOT}/tests/bin"
#pathadd PYTHONPATH "${SWATCH_ROOT}/tests/src/python"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/core/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/board/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/system/lib"
pathadd LD_LIBRARY_PATH "${SWATCH_ROOT}/tests/lib"

export PATH LD_LIBRARY_PATH
