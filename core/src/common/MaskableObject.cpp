
#include "swatch/core/MaskableObject.hpp"


namespace swatch {
namespace core {


MaskableObject::MaskableObject(const std::string& aId) : 
  MonitorableObject(aId),
  mMasked(false)
{  
}


bool MaskableObject::isMasked() const
{
  return mMasked;
}


void MaskableObject::setMasked(bool aMask)
{
  mMasked = aMask;
}


} // namespace core
} // namespace swatch
