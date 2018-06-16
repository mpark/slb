/*
  SLB.String_View

  Copyright Michael Park, 2017
  Copyright Agustin Berge, 2017

  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
*/

#ifndef SLB_STRING_VIEW_HPP
#define SLB_STRING_VIEW_HPP

/*

Header <string_view> synopsis [string.view.synop]

namespace std {
  // [string.view.template], class template basic_string_view
  template<class charT, class traits = char_traits<charT>>
  class basic_string_view;

  // [string.view.comparison], non-member comparison functions
  template<class charT, class traits>
    constexpr bool operator==(basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  template<class charT, class traits>
    constexpr bool operator!=(basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  template<class charT, class traits>
    constexpr bool operator< (basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  template<class charT, class traits>
    constexpr bool operator> (basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  template<class charT, class traits>
    constexpr bool operator<=(basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  template<class charT, class traits>
    constexpr bool operator>=(basic_string_view<charT, traits> x,
                              basic_string_view<charT, traits> y) noexcept;
  // see [string.view.comparison], sufficient additional overloads of comparison
  // functions

  // [string.view.io], inserters and extractors
  template<class charT, class traits>
    basic_ostream<charT, traits>&
      operator<<(basic_ostream<charT, traits>& os,
                 basic_string_view<charT, traits> str);

  // basic_string_view typedef names
  using string_view    = basic_string_view<char>;
  using u16string_view = basic_string_view<char16_t>;
  using u32string_view = basic_string_view<char32_t>;
  using wstring_view   = basic_string_view<wchar_t>;

  // [string.view.hash], hash support
  template<class T> struct hash;
  template<> struct hash<string_view>;
  template<> struct hash<u16string_view>;
  template<> struct hash<u32string_view>;
  template<> struct hash<wstring_view>;

  inline namespace literals {
  inline namespace string_view_literals {
    // [string.view.literals], suffix for basic_string_view literals
    constexpr string_view    operator""sv(const char* str, size_t len) noexcept;
    constexpr u16string_view operator""sv(const char16_t* str, size_t len)
        noexcept;
    constexpr u32string_view operator""sv(const char32_t* str, size_t len)
        noexcept;
    constexpr wstring_view   operator""sv(const wchar_t* str, size_t len)
        noexcept;
  }}
}

[string.view.template] Class template basic_string_view [string.view.template]

template<class charT, class traits = char_traits<charT>>
class basic_string_view {
public:
  // types
  using traits_type            = traits;
  using value_type             = charT;
  using pointer                = value_type*;
  using const_pointer          = const value_type*;
  using reference              = value_type&;
  using const_reference        = const value_type&;
  using const_iterator         = implementation-defined;
                                 // see [string.view.iterators]
  using iterator               = const_iterator;
  using const_reverse_iterator = reverse_iterator<const_iterator>;
  using reverse_iterator       = const_reverse_iterator;
  using size_type              = size_t;
  using difference_type        = ptrdiff_t;
  static constexpr size_type npos = size_type(-1);

  // [string.view.cons], construction and assignment
  constexpr basic_string_view() noexcept;
  constexpr basic_string_view(const basic_string_view&) noexcept = default;
  constexpr basic_string_view& operator=(const basic_string_view&) noexcept =
      default;
  constexpr basic_string_view(const charT* str);
  constexpr basic_string_view(const charT* str, size_type len);

  // [string.view.iterators], iterator support
  constexpr const_iterator begin() const noexcept;
  constexpr const_iterator end() const noexcept;
  constexpr const_iterator cbegin() const noexcept;
  constexpr const_iterator cend() const noexcept;
  constexpr const_reverse_iterator rbegin() const noexcept;
  constexpr const_reverse_iterator rend() const noexcept;
  constexpr const_reverse_iterator crbegin() const noexcept;
  constexpr const_reverse_iterator crend() const noexcept;

  // [string.view.capacity], capacity
  constexpr size_type size() const noexcept;
  constexpr size_type length() const noexcept;
  constexpr size_type max_size() const noexcept;
  [[nodiscard]] constexpr bool empty() const noexcept;

  // [string.view.access], element access
  constexpr const_reference operator[](size_type pos) const;
  constexpr const_reference at(size_type pos) const;
  constexpr const_reference front() const;
  constexpr const_reference back() const;
  constexpr const_pointer data() const noexcept;

  // [string.view.modifiers], modifiers
  constexpr void remove_prefix(size_type n);
  constexpr void remove_suffix(size_type n);
  constexpr void swap(basic_string_view& s) noexcept;

  // [string.view.ops], string operations
  size_type copy(charT* s, size_type n, size_type pos = 0) const;

  constexpr basic_string_view substr(size_type pos = 0, size_type n = npos)
      const;

  constexpr int compare(basic_string_view s) const noexcept;
  constexpr int compare(size_type pos1, size_type n1, basic_string_view s)
      const;
  constexpr int compare(size_type pos1, size_type n1, basic_string_view s,
  size_type pos2, size_type n2) const;
  constexpr int compare(const charT* s) const;
  constexpr int compare(size_type pos1, size_type n1, const charT* s) const;
  constexpr int compare(size_type pos1, size_type n1, const charT* s,
      size_type n2) const;

  constexpr bool starts_with(basic_string_view x) const noexcept;
  constexpr bool starts_with(charT x) const noexcept;
  constexpr bool starts_with(const charT* x) const;
  constexpr bool ends_with(basic_string_view x) const noexcept;
  constexpr bool ends_with(charT x) const noexcept;
  constexpr bool ends_with(const charT* x) const;

  // [string.view.find], searching
  constexpr size_type find(basic_string_view s, size_type pos = 0) const
      noexcept;
  constexpr size_type find(charT c, size_type pos = 0) const noexcept;
  constexpr size_type find(const charT* s, size_type pos, size_type n) const;
  constexpr size_type find(const charT* s, size_type pos = 0) const;
  constexpr size_type rfind(basic_string_view s, size_type pos = npos) const
      noexcept;
  constexpr size_type rfind(charT c, size_type pos = npos) const noexcept;
  constexpr size_type rfind(const charT* s, size_type pos, size_type n) const;
  constexpr size_type rfind(const charT* s, size_type pos = npos) const;

  constexpr size_type find_first_of(basic_string_view s, size_type pos = 0)
      const noexcept;
  constexpr size_type find_first_of(charT c, size_type pos = 0) const noexcept;
  constexpr size_type find_first_of(const charT* s, size_type pos, size_type n)
      const;
  constexpr size_type find_first_of(const charT* s, size_type pos = 0) const;
  constexpr size_type find_last_of(basic_string_view s, size_type pos = npos)
      const noexcept;
  constexpr size_type find_last_of(charT c, size_type pos = npos) const
      noexcept;
  constexpr size_type find_last_of(const charT* s, size_type pos, size_type n)
      const;
  constexpr size_type find_last_of(const charT* s, size_type pos = npos) const;
  constexpr size_type find_first_not_of(basic_string_view s, size_type pos = 0)
      const noexcept;
  constexpr size_type find_first_not_of(charT c, size_type pos = 0) const
      noexcept;
  constexpr size_type find_first_not_of(const charT* s, size_type pos,
      size_type n) const;
  constexpr size_type find_first_not_of(const charT* s, size_type pos = 0)
      const;
  constexpr size_type find_last_not_of(basic_string_view s, size_type pos =
      npos) const noexcept;
  constexpr size_type find_last_not_of(charT c, size_type pos = npos) const
      noexcept;
  constexpr size_type find_last_not_of(const charT* s, size_type pos,
      size_type n) const;
  constexpr size_type find_last_not_of(const charT* s, size_type pos = npos)
      const;

private:
  const_pointer data_; // exposition only
  size_type size_;     // exposition only
};

*/

#include "detail/config.hpp"
#include "detail/lib.hpp"

#include <cassert>
#include <cstddef>
#include <functional>
#include <ios>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace slb {
namespace detail {
static constexpr int
string_compare_sizes(int cmp, std::size_t lhs, std::size_t rhs) noexcept {
  return cmp != 0 ? cmp : lhs == rhs ? 0 : lhs < rhs ? -1 : +1;
}
} // namespace detail

// [string.view.template], class template basic_string_view
template <typename CharT, typename Traits = std::char_traits<CharT>>
class basic_string_view {
  static_assert(std::is_same<typename Traits::char_type, CharT>::value,
                "Traits::char_type shall be the same as CharT");

  static std::size_t _assert_checked_size(CharT const* str) noexcept {
    assert(str != nullptr);
    (void)str;
    return std::size_t(-1);
  }

  static constexpr std::size_t _checked_size(CharT const* str) noexcept {
    return (str != nullptr) ? Traits::length(str) : _assert_checked_size(str);
  }

  static std::size_t _assert_checked_size(CharT const* str,
                                          std::size_t len) noexcept {
    assert(len == 0 || str != nullptr);
    (void)str;
    return len;
  }

  static constexpr std::size_t _checked_size(CharT const* str,
                                             std::size_t len) noexcept {
    return (len == 0 || str != nullptr) ? len : _assert_checked_size(str, len);
  }

  std::size_t _assert_checked_pos(std::size_t pos) const noexcept {
    assert(pos < _size);
    return pos;
  }

  constexpr std::size_t _checked_pos(std::size_t pos) const noexcept {
    return (pos < _size) ? pos : _assert_checked_pos(pos);
  }

  struct _unchecked_tag {
    explicit _unchecked_tag() = default;
  };

  constexpr basic_string_view(_unchecked_tag,
                              CharT const* str,
                              std::size_t size) noexcept
      : _data(str), _size(size) {}

public:
  // types
  using traits_type = Traits;
  using value_type = CharT;
  using pointer = value_type*;
  using const_pointer = value_type const*;
  using reference = value_type&;
  using const_reference = value_type const&;
  using const_iterator = const_pointer; // see [string.view.iterators]
  using iterator = const_iterator;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using reverse_iterator = const_reverse_iterator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  static constexpr size_type npos = size_type(-1);

  // [string.view.cons], construction and assignment
  constexpr basic_string_view() noexcept : _data(nullptr), _size(0) {}

  constexpr basic_string_view(basic_string_view const&) noexcept = default;

  SLB_CXX14_CONSTEXPR basic_string_view&
  operator=(basic_string_view const&) noexcept = default;

  constexpr basic_string_view(CharT const* str) noexcept
      : _data(str), _size(_checked_size(str)) {}
  constexpr basic_string_view(CharT const* str, size_type len) noexcept
      : _data(str), _size(_checked_size(str, len)) {}

  // [string.view.iterators], iterator support
  constexpr const_iterator begin() const noexcept { return _data; }
  constexpr const_iterator end() const noexcept { return _data + _size; }
  constexpr const_iterator cbegin() const noexcept { return _data; }
  constexpr const_iterator cend() const noexcept { return _data + _size; }

  constexpr const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(begin());
  }
  constexpr const_reverse_iterator crbegin() const noexcept {
    return const_reverse_iterator(end());
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return const_reverse_iterator(begin());
  }

  // [string.view.capacity], capacity
  constexpr size_type size() const noexcept { return _size; }
  constexpr size_type length() const noexcept { return _size; }
  constexpr size_type max_size() const noexcept {
    return std::numeric_limits<std::ptrdiff_t>::max();
  }
  SLB_CXX17_NODISCARD constexpr bool empty() const noexcept {
    return _size == 0;
  }

  // [string.view.access], element access
  constexpr const_reference operator[](size_type pos) const noexcept {
    return _data[_checked_pos(pos)];
  }
  constexpr const_reference at(size_type pos) const {
    return pos >= _size ? throw std::out_of_range("slb::basic_string_view::at")
                        : 0,
           _data[pos];
  }
  constexpr const_reference front() const noexcept {
    return _data[_checked_pos(0)];
  }
  constexpr const_reference back() const noexcept {
    return _data[_checked_pos(_size - 1)];
  }
  constexpr const_pointer data() const noexcept { return _data; }

  // [string.view.modifiers], modifiers
  SLB_CXX14_CONSTEXPR void remove_prefix(size_type n) noexcept {
    assert(n <= _size);
    _data += n;
    _size -= n;
  }
  SLB_CXX14_CONSTEXPR void remove_suffix(size_type n) noexcept {
    assert(n <= _size);
    _size -= n;
  }
  SLB_CXX14_CONSTEXPR void swap(basic_string_view& s) noexcept {
    basic_string_view t(*this);
    _data = s._data;
    _size = s._size;
    s._data = t._data;
    s._size = t._size;
  }

  // [string.view.ops], string operations
  size_type copy(CharT* s, size_type n, size_type pos = 0) const {
    if (pos > _size)
      throw std::out_of_range("slb::basic_string_view::copy");

    size_type const rlen = (detail::lib::min)(n, _size - pos);
    assert(s != nullptr || rlen == 0);
    assert(_data + pos < s || _data + pos >= s + rlen);
    Traits::copy(s, _data + pos, rlen);
    return rlen;
  }

  constexpr basic_string_view substr(size_type pos = 0,
                                     size_type n = npos) const {
    return pos > _size
               ? throw std::out_of_range("slb::basic_string_view::substr")
               : 0,
           basic_string_view(_unchecked_tag{},
                             _data + pos,
                             (detail::lib::min)(n, _size - pos));
  }

  constexpr int compare(basic_string_view str) const noexcept {
    return detail::string_compare_sizes(
        Traits::compare(_data, str._data, (detail::lib::min)(_size, str._size)),
        _size,
        str._size);
  }
  constexpr int
  compare(size_type pos1, size_type n1, basic_string_view str) const noexcept {
    return substr(pos1, n1).compare(str);
  }
  constexpr int compare(size_type pos1,
                        size_type n1,
                        basic_string_view str,
                        size_type pos2,
                        size_type n2) const noexcept {
    return substr(pos1, n1).compare(str.substr(pos2, n2));
  }
  constexpr int compare(CharT const* s) const noexcept {
    return compare(basic_string_view(s));
  }
  constexpr int compare(size_type pos1, size_type n1, CharT const* s) const
      noexcept {
    return substr(pos1, n1).compare(basic_string_view(s));
  }
  constexpr int
  compare(size_type pos1, size_type n1, CharT const* s, size_type n2) const
      noexcept {
    return substr(pos1, n1).compare(basic_string_view(s, n2));
  }

  constexpr bool starts_with(basic_string_view x) const noexcept {
    return substr(0, x.size()) == x;
  }
  constexpr bool starts_with(CharT x) const noexcept {
    return !empty() && Traits::eq(front(), x);
  }
  constexpr bool starts_with(CharT const* x) const noexcept {
    return starts_with(basic_string_view(x));
  }
  constexpr bool ends_with(basic_string_view x) const noexcept {
    return size() >= x.size() && compare(size() - x.size(), npos, x) == 0;
  }
  constexpr bool ends_with(CharT x) const noexcept {
    return !empty() && Traits::eq(back(), x);
  }
  constexpr bool ends_with(CharT const* x) const noexcept {
    return ends_with(basic_string_view(x));
  }

  // [string.view.find], searching
  SLB_CXX14_CONSTEXPR size_type find(basic_string_view str,
                                     size_type pos = 0) const noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last =
        _data + (detail::lib::min)(_size - str._size, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      if (Traits::compare(xpos, str._data, str._size) == 0)
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find(CharT c, size_type pos = 0) const
      noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last = _data + (detail::lib::min)(_size - 1, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      if (Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find(CharT const* s,
                                     size_type pos,
                                     size_type n) const noexcept {
    return find(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type find(CharT const* s, size_type pos = 0) const
      noexcept {
    return find(basic_string_view(s), pos);
  }

  SLB_CXX14_CONSTEXPR size_type rfind(basic_string_view str,
                                      size_type pos = npos) const noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - str._size, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      if (Traits::compare(xpos, str._data, str._size) == 0)
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type rfind(CharT c, size_type pos = npos) const
      noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - 1, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      if (Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type rfind(CharT const* s,
                                      size_type pos,
                                      size_type n) const noexcept {
    return rfind(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type rfind(CharT const* s,
                                      size_type pos = npos) const noexcept {
    return rfind(basic_string_view(s), pos);
  }

  SLB_CXX14_CONSTEXPR size_type find_first_of(basic_string_view str,
                                              size_type pos = 0) const
      noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last = _data + (detail::lib::min)(_size - 1, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      for (size_type i = 0; i < str._size; ++i)
        if (Traits::eq(*xpos, str._data[i]))
          return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_first_of(CharT c, size_type pos = 0) const
      noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last = _data + (detail::lib::min)(_size - 1, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      if (Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_first_of(CharT const* s,
                                              size_type pos,
                                              size_type n) const noexcept {
    return find_first_of(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type find_first_of(CharT const* s,
                                              size_type pos = 0) const
      noexcept {
    return find_first_of(basic_string_view(s), pos);
  }

  SLB_CXX14_CONSTEXPR size_type find_last_of(basic_string_view str,
                                             size_type pos = npos) const
      noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - 1, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      for (size_type i = 0; i < str._size; ++i)
        if (Traits::eq(*xpos, str._data[i]))
          return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_last_of(CharT c,
                                             size_type pos = npos) const
      noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - 1, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      if (Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_last_of(CharT const* s,
                                             size_type pos,
                                             size_type n) const noexcept {
    return find_last_of(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type find_last_of(CharT const* s,
                                             size_type pos = npos) const
      noexcept {
    return find_last_of(basic_string_view(s), pos);
  }

  SLB_CXX14_CONSTEXPR size_type find_first_not_of(basic_string_view str,
                                                  size_type pos = 0) const
      noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last = _data + (detail::lib::min)(_size - 1, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      size_type i = 0;
      while (i < str._size && !Traits::eq(*xpos, str._data[i]))
        ++i;
      if (i == str._size)
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_first_not_of(CharT c,
                                                  size_type pos = 0) const
      noexcept {
    const_pointer const first = _data + (detail::lib::min)(pos, _size);
    const_pointer const last = _data + (detail::lib::min)(_size - 1, _size);
    for (const_pointer xpos = first; xpos < last; ++xpos) {
      if (!Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_first_not_of(CharT const* s,
                                                  size_type pos,
                                                  size_type n) const noexcept {
    return find_first_not_of(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type find_first_not_of(CharT const* s,
                                                  size_type pos = 0) const
      noexcept {
    return find_first_not_of(basic_string_view(s), pos);
  }

  SLB_CXX14_CONSTEXPR size_type find_last_not_of(basic_string_view str,
                                                 size_type pos = npos) const
      noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - 1, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      size_type i = 0;
      while (i < str._size && !Traits::eq(*xpos, str._data[i]))
        ++i;
      if (i == str._size)
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_last_not_of(CharT c,
                                                 size_type pos = npos) const
      noexcept {
    const_pointer const first = _data;
    const_pointer const last =
        _data + (detail::lib::min)(pos, _size - 1, _size);
    for (const_pointer xpos = last; xpos >= first; --xpos) {
      if (!Traits::eq(*xpos, c))
        return xpos - _data;
    }
    return npos;
  }
  SLB_CXX14_CONSTEXPR size_type find_last_not_of(CharT const* s,
                                                 size_type pos,
                                                 size_type n) const noexcept {
    return find_last_not_of(basic_string_view(s, n), pos);
  }
  SLB_CXX14_CONSTEXPR size_type find_last_not_of(CharT const* s,
                                                 size_type pos = npos) const
      noexcept {
    return find_last_not_of(basic_string_view(s), pos);
  }

private:
  const_pointer _data;
  size_type _size;
};

template <typename CharT, typename Traits>
constexpr typename basic_string_view<CharT, Traits>::size_type
    basic_string_view<CharT, Traits>::npos;

// [string.view.comparison], non-member comparison functions
namespace detail {
template <typename U, typename CharT, typename Traits>
struct enable_string_view_convertible
    : std::enable_if<
          !std::is_same<typename detail::lib::remove_cvref<U>::type,
                        basic_string_view<CharT, Traits>>::value &&
          std::is_convertible<U, basic_string_view<CharT, Traits>>::value> {};
} // namespace detail

template <typename CharT, typename Traits>
constexpr bool operator==(basic_string_view<CharT, Traits> x,
                          basic_string_view<CharT, Traits> y) noexcept {
  return x.size() == y.size() && x.compare(y) == 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator==(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) == y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator==(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x == basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

template <typename CharT, typename Traits>
constexpr bool operator!=(basic_string_view<CharT, Traits> x,
                          basic_string_view<CharT, Traits> y) noexcept {
  return x.size() != y.size() || x.compare(y) != 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator!=(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) != y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator!=(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x != basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

template <typename CharT, typename Traits>
constexpr bool operator<(basic_string_view<CharT, Traits> x,
                         basic_string_view<CharT, Traits> y) noexcept {
  return x.compare(y) < 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator<(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) < y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator<(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x < basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

template <typename CharT, typename Traits>
constexpr bool operator>(basic_string_view<CharT, Traits> x,
                         basic_string_view<CharT, Traits> y) noexcept {
  return x.compare(y) > 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator>(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) > y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator>(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x > basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

template <typename CharT, typename Traits>
constexpr bool operator<=(basic_string_view<CharT, Traits> x,
                          basic_string_view<CharT, Traits> y) noexcept {
  return x.compare(y) <= 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator<=(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) <= y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator<=(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x <= basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

template <typename CharT, typename Traits>
constexpr bool operator>=(basic_string_view<CharT, Traits> x,
                          basic_string_view<CharT, Traits> y) noexcept {
  return x.compare(y) >= 0;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator>=(U&& x, basic_string_view<CharT, Traits> y) noexcept {
  return basic_string_view<CharT, Traits>(static_cast<U&&>(x)) >= y;
}
template <
    typename CharT,
    typename Traits,
    typename U,
    typename Enable =
        typename detail::enable_string_view_convertible<U, CharT, Traits>::type>
constexpr bool operator>=(basic_string_view<CharT, Traits> x, U&& y) noexcept {
  return x >= basic_string_view<CharT, Traits>(static_cast<U&&>(y));
}

// [string.view.io], inserters and extractors
namespace detail {
template <typename CharT, typename Traits>
bool string_io_pad(std::basic_ostream<CharT, Traits>& os,
                   std::streamsize const pad) {
  CharT const fill = os.fill();
  CharT const chunk[] = {
      fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill, fill};
  std::streamsize constexpr chunk_size = sizeof(chunk) / sizeof(CharT);

  bool good = true;
  for (std::streamsize i = 0; i < pad; i += chunk_size) {
    std::streamsize const rlen = (detail::lib::min)(chunk_size, pad - i);
    good = good && os.rdbuf()->sputn(chunk, rlen);
  }
  return good;
}
} // namespace detail

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           basic_string_view<CharT, Traits> str) {
  typename std::basic_ostream<CharT, Traits>::sentry sentry(os);

  bool good(sentry);
  if (good) {
    try {
      std::streamsize const len = str.size();
      std::streamsize const pad = os.width() - len;
      bool const left_pad =
          (os.flags() & std::ios_base::adjustfield) == std::ios_base::left;

      good = good && (pad < 0 || !left_pad || detail::string_io_pad(os, pad));
      good = good && os.rdbuf()->sputn(str.data(), len) == len;
      good = good && (pad < 0 || left_pad || detail::string_io_pad(os, pad));
    } catch (...) {
      os.setstate(std::ios_base::badbit);
    }
  }
  if (!good) {
    os.setstate(std::ios_base::failbit);
  }
  os.width(0); // reset width

  return os;
}

// basic_string_view typedef names
using string_view = basic_string_view<char>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
using wstring_view = basic_string_view<wchar_t>;

// [string.view.hash], hash support
namespace detail {
template <typename CharT>
std::size_t string_hash(CharT const* data, std::size_t len) {
  CharT const* const first = data;
  CharT const* const last = data + len;

  std::size_t result = 2166136261;
  for (CharT const* ptr = first; ptr < last; ++ptr) {
    result = (result * 16777619) ^ *ptr;
  }
  return result;
}
} // namespace detail

template <typename T>
struct hash;

template <>
struct hash<string_view> {
  std::size_t operator()(string_view str) const noexcept {
    return detail::string_hash(str.data(), str.length());
  }
};

template <>
struct hash<u16string_view> {
  std::size_t operator()(u16string_view str) const noexcept {
    return detail::string_hash(str.data(), str.length());
  }
};

template <>
struct hash<u32string_view> {
  std::size_t operator()(u32string_view str) const noexcept {
    return detail::string_hash(str.data(), str.length());
  }
};

template <>
struct hash<wstring_view> {
  std::size_t operator()(wstring_view str) const noexcept {
    return detail::string_hash(str.data(), str.length());
  }
};

inline namespace literals {
inline namespace string_view_literals {
// [string.view.literals], suffix for basic_string_view literals
constexpr string_view operator"" _sv(char const* str,
                                     std::size_t len) noexcept {
  return string_view{str, len};
}
constexpr u16string_view operator"" _sv(char16_t const* str,
                                        std::size_t len) noexcept {
  return u16string_view{str, len};
}
constexpr u32string_view operator"" _sv(char32_t const* str,
                                        std::size_t len) noexcept {
  return u32string_view{str, len};
}
constexpr wstring_view operator"" _sv(wchar_t const* str,
                                      std::size_t len) noexcept {
  return wstring_view{str, len};
}
} // namespace string_view_literals
} // namespace literals
} // namespace slb

namespace std {
template <>
struct hash<slb::string_view> : slb::hash<slb::string_view> {};
template <>
struct hash<slb::u16string_view> : slb::hash<slb::u16string_view> {};
template <>
struct hash<slb::u32string_view> : slb::hash<slb::u32string_view> {};
template <>
struct hash<slb::wstring_view> : slb::hash<slb::wstring_view> {};
} // namespace std

#endif // SLB_STRING_VIEW_HPP
