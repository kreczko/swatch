/**
 * @file    StatusFlag.hpp
 * @author  Tom Williams
 * @date    May 2015
 */

#ifndef __SWATCH_CORE_STATUSFLAG_HPP__
#define	__SWATCH_CORE_STATUSFLAG_HPP__


#include <iosfwd>


namespace swatch {
namespace core {


enum StatusFlag{
    kGood,
    kError,
    kUnknown
};

} // namespace core
} // namespace swatch

std::ostream& operator<<(std::ostream& out, swatch::core::StatusFlag value);


swatch::core::StatusFlag operator& (const swatch::core::StatusFlag& flag1, const swatch::core::StatusFlag& flag2);


#endif	/* __SWATCH_CORE_STATUSFLAG_HPP__ */

