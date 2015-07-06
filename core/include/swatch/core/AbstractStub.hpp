/**
 * @file    AbstractStub.hpp
 * @author  Alessandro Thea
 * @brief   Template class to provide support for abstract classes
 * @date    July 2015
 *
 */

#ifndef __SWATCH_CORE_ABSTRACTSTUB_HPP__
#define	__SWATCH_CORE_ABSTRACTSTUB_HPP__

// C++ Headers
#include <string>

namespace swatch {
namespace core {

class AbstractStub {
public:
  explicit AbstractStub( const std::string& aId ) : id(aId) {}
  virtual ~AbstractStub() {}

  std::string id;
};

}
}

#endif	/* __SWATCH_CORE_ABSTRACTSTUB_HPP__ */

