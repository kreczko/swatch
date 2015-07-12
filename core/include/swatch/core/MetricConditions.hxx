/**
 * @file    MetricConditions.hxx
 * @author  Tom Williams
 * @date    June 2015
 */

#ifndef __SWATCH_CORE_METRICCONDITIONS_HXX__
#define __SWATCH_CORE_METRICCONDITIONS_HXX__


#include <iostream>

#include "MetricConditions.hpp"


namespace swatch {
namespace core {

/* ---  EqualCondition  --- */

template<typename DataType>
EqualCondition<DataType>::EqualCondition(const DataType& aValue) : 
value_(aValue) {
}

template<typename DataType>
EqualCondition<DataType>::~EqualCondition() {}

template<typename DataType>
bool EqualCondition<DataType>::operator()(const DataType& aValue) const {
  return (value_ == aValue);
}

template<typename DataType>
void EqualCondition<DataType>::print(std::ostream& aStream) const {
  aStream << " == " << value_;
}


/* ---  NotEqualCondition  --- */

template<typename DataType>
NotEqualCondition<DataType>::NotEqualCondition(const DataType& aValue) : 
value_(aValue) {
}

template<typename DataType>
NotEqualCondition<DataType>::~NotEqualCondition() {}

template<typename DataType>
bool NotEqualCondition<DataType>::operator()(const DataType& aValue) const {
  return (value_ != aValue);
}

template<typename DataType>
void NotEqualCondition<DataType>::print(std::ostream& aStream) const {
  aStream << " != " << value_;
}


/* ---  RangeCondition  --- */


template<typename DataType>
RangeCondition<DataType>::RangeCondition(const DataType& aLower, const DataType& aUpper) : 
  lower_(aLower),
  upper_(aUpper) {
}

template<typename DataType>
RangeCondition<DataType>::~RangeCondition() {}

template<typename DataType>
bool RangeCondition<DataType>::operator()(const DataType& aValue) const {
  return ( (aValue >= lower_) && (aValue <= upper_) );
}

template<typename DataType>
void RangeCondition<DataType>::print(std::ostream& aStream) const {
  aStream << "[" << lower_ << ", " << upper_ << "]";
}


/* ---  InvRangeCondition  --- */


template<typename DataType>
InvRangeCondition<DataType>::InvRangeCondition(const DataType& aLower, const DataType& aUpper) : 
  lower_(aLower),
  upper_(aUpper) {
}

template<typename DataType>
InvRangeCondition<DataType>::~InvRangeCondition() {}

template<typename DataType>
bool InvRangeCondition<DataType>::operator()(const DataType& aValue) const {
  return ( (aValue < lower_) || (aValue > upper_) );
}

template<typename DataType>
void InvRangeCondition<DataType>::print(std::ostream& aStream) const {
  aStream << " < " << lower_ << " or > " << upper_;
}


/* ---  LessThanCondition  --- */


template<typename DataType>
LessThanCondition<DataType>::LessThanCondition(const DataType& aLimit) : 
  limit_(aLimit) {
}

template<typename DataType>
LessThanCondition<DataType>::~LessThanCondition() {}

template<typename DataType>
bool LessThanCondition<DataType>::operator()(const DataType& aValue) const {
  return (aValue < limit_);
}

template<typename DataType>
void LessThanCondition<DataType>::print(std::ostream& aStream) const {
  aStream << "< " << limit_ ;
}


/* ---  GreaterThanCondition  --- */


template<typename DataType>
GreaterThanCondition<DataType>::GreaterThanCondition(const DataType& aLimit) :
  limit_(aLimit) {
}

template<typename DataType>
GreaterThanCondition<DataType>::~GreaterThanCondition() {}

template<typename DataType>
bool GreaterThanCondition<DataType>::operator()(const DataType& aValue) const {
  return (aValue > limit_);
}

template<typename DataType>
void GreaterThanCondition<DataType>::print(std::ostream& aStream) const {
  aStream << "> " << limit_;
}


} // namespace core
} // namespace swatch



#endif	/* __SWATCH_CORE_METRICCONDITIONS_HPP__ */

