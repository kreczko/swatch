/* 
 * File:   DescriptorSelector.hpp
 * Author: ale
 *
 * Created on March 6, 2016, 12:35 PM
 */

#ifndef __SWATCH_MP7_CHANNEL_DESCRIPTORFINDER_HPP__
#define	__SWATCH_MP7_CHANNEL_DESCRIPTORFINDER_HPP__

#include "swatch/core/exception.hpp"
#include <boost/noncopyable.hpp>

#include "swatch/mp7/channel/Descriptor.hpp"

namespace swatch {
namespace mp7 {
namespace channel {

// Typedefs
typedef boost::function<bool (const Descriptor&) > Rule_t;

class DescriptorFinder : public boost::noncopyable {
public:

  DescriptorFinder( const std::map<std::string, Descriptor>& aMap);

  virtual ~DescriptorFinder();

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
  void checkExist( const std::set<std::string>& ) const;
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
  const DescriptorMap_t& mMap;
};

DEFINE_SWATCH_EXCEPTION(DescriptorIdsNotFound);

} // namespace channel
} // namespace mp7 
} // namespace swatch

#endif	/* __SWATCH_MP7_CHANNEL_DESCRIPTORFINDER_HPP__ */

