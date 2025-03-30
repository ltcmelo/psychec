/******************************************************************************
 The MIT License

 Copyright (c) 2009-2015 Leandro T. C. Melo

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/

#ifndef BASIC_STRING_UTILS_CASE_CONV_HPP
#define BASIC_STRING_UTILS_CASE_CONV_HPP

#include "config.hpp"
#include "detail.hpp"
#include <string>
#include <locale>
#include <algorithm>

NAMESPACE_BEGIN

template <class allocator_t>
void
to_lower(std::basic_string<char, std::char_traits<char>, allocator_t> & input)
{
  std::for_each(input.begin(), input.end(), detail::ascii_to_lower);
}

template <class char_t, class traits_t, class allocator_t>
void
to_lower(std::basic_string<char_t, traits_t, allocator_t> & input, std::locale const& loc)
{
  char_t * low = &*input.begin();
  std::use_facet<std::ctype<char_t> >(loc).tolower(low, low + input.length());
}

template <class allocator_t>
std::basic_string<char, std::char_traits<char>, allocator_t>
to_lower_copy(std::basic_string<char, std::char_traits<char>, allocator_t> const& input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  string_type copy(input);
  to_lower(copy);
  return copy;
}

template <class char_t, class traits_t, class allocator_t>
std::basic_string<char_t, traits_t, allocator_t>
to_lower_copy(std::basic_string<char_t, traits_t, allocator_t> const& input, std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;

  string_type copy(input);
  to_lower(copy, loc);
  return copy;
}

template <class allocator_t>
void
to_upper(std::basic_string<char, std::char_traits<char>, allocator_t> & input)
{
  std::for_each(input.begin(), input.end(), detail::ascii_to_upper);
}

template <class char_t, class traits_t, class allocator_t>
void
to_upper(std::basic_string<char_t, traits_t, allocator_t> & input, std::locale const& loc)
{
  char_t * low = &*input.begin();
  std::use_facet<std::ctype<char_t> >(loc).toupper(low, low + input.length());
}

template <class allocator_t>
std::basic_string<char, std::char_traits<char>, allocator_t>
to_upper_copy(std::basic_string<char, std::char_traits<char>, allocator_t> const& input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  string_type copy(input);
  to_upper(copy);
  return copy;
}

template <class char_t, class traits_t, class allocator_t>
std::basic_string<char_t, traits_t, allocator_t>
to_upper_copy(std::basic_string<char_t, traits_t, allocator_t> const& input, std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;

  string_type copy(input);
  to_upper(copy, loc);
  return copy;
}

NAMESPACE_END

#endif
