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

#ifndef BASIC_STRING_UTILS_TRIM_HPP
#define BASIC_STRING_UTILS_TRIM_HPP

#include "config.hpp"
#include "detail.hpp"
#include <string>
#include <locale>
#include <algorithm>

NAMESPACE_BEGIN

template <class allocator_t>
void
trim_left(std::basic_string<char, std::char_traits<char>, allocator_t> & input)
{
  input.erase(0, input.find_first_not_of(detail::space)); //The C++ standard requires that an
    //out_of_range exception is thrown only if pos > size(). So passing 0 is OK even for an
    //empty string.
}

template <class char_t, class traits_t, class allocator_t>
void
trim_left(std::basic_string<char_t, traits_t, allocator_t> & input, std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;
  typedef typename string_type::iterator iterator;

  iterator it = std::find_if(input.begin(), input.end(), detail::not_space<char_t>(loc));
  input.erase(input.begin(), it);
}

template <class allocator_t>
void
trim_right(std::basic_string<char, std::char_traits<char>, allocator_t> & input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  std::size_t pos = input.find_last_not_of(detail::space);
  if (pos == string_type::npos)
    input.erase(0);
  else if (++pos != input.size())
    input.erase(pos);
}

template <class char_t, class traits_t, class allocator_t>
void
trim_right(std::basic_string<char_t, traits_t, allocator_t> & input, std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;
  typedef typename string_type::reverse_iterator reverse_iterator;

  reverse_iterator it = std::find_if(input.rbegin(), input.rend(), detail::not_space<char_t>(loc));
  input.erase(it.base(), input.end());
}

template <class allocator_t>
void
trim(std::basic_string<char, std::char_traits<char>, allocator_t> & input)
{
  trim_left(input);
  trim_right(input);
}


template <class char_t, class traits_t, class allocator_t>
void
trim(std::basic_string<char_t, traits_t, allocator_t> & input, std::locale const& loc)
{
  trim_left(input, loc);
  trim_right(input, loc);
}

template <class allocator_t>
std::basic_string<char, std::char_traits<char>, allocator_t>
trim_left_copy(std::basic_string<char, std::char_traits<char>, allocator_t> const& input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  string_type copy(input);
  trim_left(copy);
  return copy;
}

template <class char_t, class traits_t, class allocator_t>
std::basic_string<char_t, traits_t, allocator_t>
trim_left_copy(std::basic_string<char_t, traits_t, allocator_t> const& input,
               std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;

  string_type copy(input);
  trim_left(copy, loc);
  return copy;
}

template <class allocator_t>
std::basic_string<char, std::char_traits<char>, allocator_t>
trim_right_copy(std::basic_string<char, std::char_traits<char>, allocator_t> const& input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  string_type copy(input);
  trim_right(copy);
  return copy;
}

template <class char_t, class traits_t, class allocator_t>
std::basic_string<char_t, traits_t, allocator_t>
trim_right_copy(std::basic_string<char_t, traits_t, allocator_t> const& input,
                std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;

  string_type copy(input);
  trim_right(copy, loc);
  return copy;
}

template <class allocator_t>
std::basic_string<char, std::char_traits<char>, allocator_t>
trim_copy(std::basic_string<char, std::char_traits<char>, allocator_t> const& input)
{
  typedef std::basic_string<char, std::char_traits<char>, allocator_t> string_type;

  string_type copy(input);
  trim(copy);
  return copy;
}

template <class char_t, class traits_t, class allocator_t>
std::basic_string<char_t, traits_t, allocator_t>
trim_copy(std::basic_string<char_t, traits_t, allocator_t> const& input,
          std::locale const& loc)
{
  typedef std::basic_string<char_t, traits_t, allocator_t> string_type;

  string_type copy(input);
  trim(copy, loc);
  return copy;
}

NAMESPACE_END

#endif
