#ifndef __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__
#define __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__

// MP7 Headers
#include "mp7/definitions.hpp"

#include <set>

namespace swatch {

namespace core {
  class MaskableObject;
} // namespace core

namespace mp7 {
namespace channel {

// @class Helper class to describe for MP7 channel properties
class Descriptor {
public:
  
  /**
   * Default constructor
   */
  Descriptor();

  /**
   * Constructor. Doesn't rely on the channel to be mapped on a port
   * In case of future extensions
   * @param aId
   * @param aChannelId
   * @param aHasMGTs
   * @param aHasBuffer
   * @param aFmtKind
   */
  Descriptor(uint32_t aChannelId, bool aHasMGTs, bool aHasBuffer, ::mp7::FormatterKind aFmtKind, const core::MaskableObject* aMaskable = 0);
  uint32_t getChannelId() const;
  bool hasMGT() const;
  bool hasBuffer() const;
  ::mp7::FormatterKind getFormatterKind() const;
  bool isMasked() const;

private:
  uint32_t mChannelId;
  bool mHasMGT;
  bool mHasBuffer;
  ::mp7::FormatterKind mFormatterKind;
  const core::MaskableObject* mMaskable;

};


typedef std::map<std::string, Descriptor> DescriptorMap_t;

} // namespace channel
} // namespace mp7 
} // namespace swatch

#endif /* __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__ */