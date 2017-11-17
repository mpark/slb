/*
  SLB.TypeTraits

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_DETAIL_LIB_HPP
#define SLB_DETAIL_LIB_HPP

#include <type_traits>

namespace slb {
namespace detail {
namespace lib {

template <typename>
struct always_false : std::false_type {};

} // namespace lib
} // namespace detail
} // namespace slb

#endif // SLB_DETAIL_LIB_HPP
