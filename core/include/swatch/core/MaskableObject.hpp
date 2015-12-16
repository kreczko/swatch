/* 
 * @file    MaskableObject.hpp
 * @author  Tom Williams
 * @date    December 2015
 */

#ifndef SWATCH_CORE_MASKABLEOBJECT_HPP
#define	SWATCH_CORE_MASKABLEOBJECT_HPP

// SWATCH headers
#include "swatch/core/MonitorableObject.hpp"


namespace swatch {
namespace core {


class MaskableObject : public MonitorableObject {
public:
  
  explicit MaskableObject(const std::string& aId);

  bool isMasked() const;
  
  void setMasked(bool aMask=true);

private:
  bool mMasked;
};


} // namespace core
} // namespace swatch


#endif	/* SWATCH_CORE_MASKABLEOBJECT_HPP */

