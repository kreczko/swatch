#include "swatch/mp7/ChannelDescriptor.hpp"

// SWATCH Headers
#include "swatch/core/MaskableObject.hpp"

// BOOST Headers

#include <boost/foreach.hpp>
#include <stdexcept>
#include <sstream>

namespace swatch {
namespace mp7 {


ChannelDescriptor::ChannelDescriptor()
{

}

ChannelDescriptor::ChannelDescriptor(uint32_t aChannelId, bool aHasMGTs, bool aHasBuffer, ::mp7::FormatterKind aFmtKind, const core::MaskableObject* aMaskable) :
  mChannelId(aChannelId),
  mHasMGT(aHasMGTs),
  mHasBuffer(aHasBuffer),
  mFormatterKind(aFmtKind),
  mMaskable(aMaskable)
{
//  std::cout << "--------------------" << std::endl;
//  std::cout << "objid " << mObjId << std::endl;
//  std::cout << "chid  " << mChannelId << std::endl;
//  std::cout << "mgt  " << mHasMGT << std::endl;
//  std::cout << "buf  " << mHasBuffer << std::endl;
  
}

uint32_t ChannelDescriptor::getChannelId() const
{
  return mChannelId;
}

bool ChannelDescriptor::hasMGT() const
{
  return mHasMGT;
}

bool ChannelDescriptor::hasBuffer() const
{
  return mHasBuffer;
}


::mp7::FormatterKind ChannelDescriptor::getFormatterKind() const
{
  return mFormatterKind;
}

bool ChannelDescriptor::isMasked() const
{
  if ( !mMaskable ) return false;
  
  return mMaskable->isMasked();
}


DescriptorSelector::DescriptorSelector(const std::map<std::string, ChannelDescriptor>& aMap) :
  mMap(aMap)
{
}


DescriptorSelector::~DescriptorSelector()
{
}

std::set<std::string> DescriptorSelector::getIds() const
{
  std::set<std::string> lIds;
  boost::copy(mMap
      | boost::adaptors::map_keys,
      std::inserter(lIds, lIds.end()));
  return lIds;
}

std::vector<uint32_t> DescriptorSelector::getChannelIds() const
{
  std::vector<uint32_t> lIds;
  boost::copy(mMap 
    | boost::adaptors::map_values 
    | boost::adaptors::transformed(boost::bind(&ChannelDescriptor::getChannelId,_1)),
    std::inserter(lIds, lIds.end()));
  return lIds;
}


std::vector<uint32_t> DescriptorSelector::mapIdsToChannels(const std::set<std::string>& aIds) const
{
  
  checkAvailable(aIds);
  
  std::vector<uint32_t> lChIds;
  BOOST_FOREACH(const std::string& id, aIds) {
    lChIds.push_back(mMap.at(id).getChannelId());
  }
    
  return lChIds;
}


std::set<std::string> DescriptorSelector::queryIds(const Rule_t& aPred) const
{
  auto lArg = boost::bind(&std::map<std::string, ChannelDescriptor>::value_type::second, _1);
  std::set<std::string> lIds;
  boost::copy(mMap
      | boost::adaptors::filtered(boost::bind(aPred, lArg))
      | boost::adaptors::map_keys,
      std::inserter(lIds, lIds.end()));
  return lIds;

}


void DescriptorSelector::checkAvailable(const std::set<std::string>& aIds ) const
{
  std::set<std::string> lIds = getIds();

  std::set<std::string> lNotFound;
  std::set_difference (aIds.begin(), aIds.end(), lIds.begin(), lIds.end(), std::inserter(lNotFound, lNotFound.end()));

  if ( !lNotFound.empty() ) {
    std::ostringstream msg;
    std::copy(lNotFound.begin(), lNotFound.end(), std::ostream_iterator<std::string>(msg, " "));
    throw DescriptorIdsNotFound(msg.str());
  }
}

std::set<std::string> DescriptorSelector::filterIds( const std::set<std::string>& aIds, const Rule_t& aPred ) const
{
  std::set<std::string> lFiltered;
  std::vector<std::string> lNotFound;
  BOOST_FOREACH( const std::string& id, aIds ) {
    try {
      if ( !aPred(mMap.at(id)) ) continue;
      lFiltered.insert(id);
    } catch ( const std::out_of_range& e ) {
      lNotFound.push_back(id);
    }
  }
  
  if ( !lNotFound.empty() ) {
    std::ostringstream msg;
    std::copy(lNotFound.begin(), lNotFound.end(), std::ostream_iterator<std::string>(msg, " "));
    throw DescriptorIdsNotFound(msg.str());
  }
  
  return lFiltered;
}

} // namespace mp7
} // namespace swatch