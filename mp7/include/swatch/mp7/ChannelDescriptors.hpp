#ifndef __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__
#define __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__

// MP7 Headers
#include "mp7/definitions.hpp"

#include <boost/noncopyable.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/copy.hpp>

#include "swatch/core/exception.hpp"

#include <set>

namespace swatch {

namespace core {
  class MaskableObject;
} // namespace core

namespace mp7 {
// Forward declaration
class ChannelDescriptor;

// Typedefs
typedef boost::function<bool (const ChannelDescriptor&) > Rule_t;


// @class Helper class to describe for MP7 channel properties
class ChannelDescriptor {
public:
  
  /**
   * Default constructor
   */
  ChannelDescriptor();

  /**
   * Constructor. Doesn't rely on the channel to be mapped on a port
   * In case of future extensions
   * @param aId
   * @param aChannelId
   * @param aHasMGTs
   * @param aHasBuffer
   * @param aFmtKind
   */
  ChannelDescriptor(uint32_t aChannelId, bool aHasMGTs, bool aHasBuffer, ::mp7::FormatterKind aFmtKind, const core::MaskableObject* aMaskable = 0);
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


typedef std::map<std::string, ChannelDescriptor> ChannelsMap_t;

class DescriptorSelector : public boost::noncopyable {
public:

  DescriptorSelector( const std::map<std::string, ChannelDescriptor>& aMap);

  virtual ~DescriptorSelector();

  //----
  /**
   * Get all ids stored as a set
   * @return Set of ids
   */
  std::set<std::string> getIds() const;;
  
  /**
   * Query Ids according to selector
   * @param aSelector
   * @return 
   */
  std::set<std::string> queryIds(const Rule_t& aPred) const;
  
  /**
   * 
   * @param 
   */
  void checkAvailable( const std::set<std::string>& ) const;
  /**
   * Filter ids that match a condition
   * @param aSelector
   * @return 
   */
  std::set<std::string> filterIds(const std::set<std::string>& aIds, const Rule_t& aPred) const;

  /**
   * 
   * @return 
   */
  std::vector<uint32_t> getChannelIds() const;
  
  /**
   * 
   * @param 
   * @return 
   */
  std::vector<uint32_t> mapIdsToChannels( const std::set<std::string>& ) const;
  
private:
  const ChannelsMap_t& mMap;
};

DEFINE_SWATCH_EXCEPTION(DescriptorIdsNotFound);


} // namespace mp7 
} // namespace swatch

#endif /* __SWATCH_MP7_CHANNELDESCRIPTORS_HPP__ */