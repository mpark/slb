/*
  SLB.Detail.Config

  Copyright Michael Park, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_DETAIL_CONFIG_HPP
#define SLB_DETAIL_CONFIG_HPP

// MSVC 2015 Update 3.
#if __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_FULL_VER < 190024210)
#error "SLB requires C++11 support."
#endif

#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#ifndef __has_include
#define __has_include(x) 0
#endif

#ifndef __has_feature
#define __has_feature(x) 0
#endif

#ifndef __has_warning
#define __has_warning(x) 0
#endif

// N3652: "Relaxing constraints on constexpr functions / constexpr member
// functions and implicit const"
#if (defined(__cpp_constexpr) && __cpp_constexpr >= 201304) ||                 \
    (defined(_MSC_VER) && _MSC_VER >= 1910)
#define SLB_CXX14_CONSTEXPR constexpr
#else
#define SLB_CXX14_CONSTEXPR
#endif

// N3651: "Variable Templates"
#if (defined(__cpp_variable_templates) &&                                      \
     __cpp_variable_templates >= 201304) ||                                    \
    (defined(_MSC_VER) && _MSC_VER >= 1900)
#define SLB_HAS_CXX14_VARIABLE_TEMPLATES 1
#else
#define SLB_HAS_CXX14_VARIABLE_TEMPLATES 0
#endif

// P0386: "Inline Variables"
#if (defined(__cpp_inline_variables) && __cpp_inline_variables >= 201606) ||   \
    (defined(_MSC_VER) && _MSC_VER >= 1912 && _HAS_CXX17)
#define SLB_CXX17_INLINE_VARIABLE inline
#else
#define SLB_CXX17_INLINE_VARIABLE
#endif

#endif // SLB_DETAIL_CONFIG_HPP
