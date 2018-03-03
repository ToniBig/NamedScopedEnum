// The MIT License (MIT)
//
// Copyright (c) 2016-2018 Nils Zander, Tino Bog
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef _NAMED_SCOPED_ENUM_HPP_
#define _NAMED_SCOPED_ENUM_HPP_

#include <string>
#include <vector>

namespace detail {

constexpr size_t count_impl( const char * string,
                             char character,
                             size_t char_count,
                             size_t comma_count ){
  if ( string[--char_count] == character ) comma_count++;
  if ( char_count == 0 ) return comma_count;
  return count_impl( string, character, char_count, comma_count );
}

template<size_t N>
constexpr size_t count_character( char const (&string)[N],
                                  char character ){
  return detail::count_impl( string, character, N, 0 );
}

template<size_t N>
constexpr size_t length( char const (&string)[N] ){
  return N;
}

template<size_t N>
constexpr size_t empty( char const (&string)[N] ){
  return ( N == 1 ) && string[0] == '\0';
}

using strings_t=std::vector<std::string>;

template<int N, size_t C>
class tokenizer {
  char string_[N] { };
  size_t ids_[C] { };

public:
  constexpr tokenizer( char const (&string)[N] ){
    size_t count = 0;
    for ( size_t i = 0; i < N; ++i ) {
      if ( string[i] == ',' ) {
        ids_[++count] = i + 2;
        string_[i] = '\0';
      }
      else {
        string_[i] = string[i];
      }
    } // end of i-loop
  }

  constexpr
  char const * operator[]( size_t i ) const{
    return &string_[ids_[i]];
  }

  constexpr operator bool( ) const{
    return true;
  }

  strings_t const & strings() const{
    static const strings_t strings=make_strings();
    return strings;
  }

private:
  strings_t make_strings( ) const{
    auto strings=strings_t(C);
    for (size_t i = 0; i < C; ++i) {
      strings[i]=this->operator [](i);
    } // end of i-loop
    return strings;
  }

};

template<size_t C, size_t N> constexpr
tokenizer<N, C> make_tokenizer( char const (&string)[N] ){
  return tokenizer<N, C>( string );
}

} // namespace detail

#define NAMED_SCOPED_ENUM(CLASS_NAME,...)                                                 \
                                                                                          \
constexpr size_t _##CLASS_NAME##_SIZE_=detail::count_character(#__VA_ARGS__,',')+1;       \
                                                                                          \
class CLASS_NAME                                                                          \
{                                                                                         \
                                                                                          \
  static_assert(!detail::empty(#__VA_ARGS__),"No enumerators provided");                  \
                                                                                          \
  static_assert(detail::count_character(#__VA_ARGS__,'=')==0,                             \
                "Custom enumerators (=) are not supported");                              \
                                                                                          \
public:                                                                                   \
                                                                                          \
  enum ENUM {__VA_ARGS__};                                                                \
                                                                                          \
  ENUM  value() const                                                                     \
  {                                                                                       \
    return enum_;                                                                         \
  }                                                                                       \
                                                                                          \
  constexpr CLASS_NAME(ENUM enumerator=ENUM{}) :                                          \
    enum_(enumerator){}                                                                   \
                                                                                          \
  static constexpr size_t size()                                                          \
  {                                                                                       \
    return _##CLASS_NAME##_SIZE_;                                                         \
  }                                                                                       \
                                                                                          \
private:                                                                                  \
  using tokens_t = decltype(detail::make_tokenizer<_##CLASS_NAME##_SIZE_>(#__VA_ARGS__)); \
  static tokens_t const & tokens()                                                        \
  {                                                                                       \
    static const auto names = detail::make_tokenizer<_##CLASS_NAME##_SIZE_>(#__VA_ARGS__);\
    return names;                                                                         \
  }                                                                                       \
                                                                                          \
public:                                                                                   \
  static std::string name(size_t i)                                                       \
  {                                                                                       \
    return tokens()[i];                                                                   \
  }                                                                                       \
                                                                                          \
  static detail::strings_t const & names()                                                \
  {                                                                                       \
    return tokens().strings();                                                            \
  }                                                                                       \
                                                                                          \
  std::string name() const                                                                \
  {                                                                                       \
    return name(enum_);                                                                   \
  }                                                                                       \
                                                                                          \
  operator std::string() const                                                            \
  {                                                                                       \
    return name(enum_);                                                                   \
  }                                                                                       \
                                                                                          \
  friend bool operator == (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ == rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
  friend bool operator != (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ != rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
  friend bool operator <  (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ <  rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
  friend bool operator <= (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ <= rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
  friend bool operator >  (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ >  rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
  friend bool operator >= (const CLASS_NAME & lhs, const CLASS_NAME & rhs)                \
  {                                                                                       \
    return lhs.enum_ >= rhs.enum_;                                                        \
  }                                                                                       \
                                                                                          \
private:                                                                                  \
  ENUM enum_;                                                                             \
}


#endif // _NAMED_SCOPED_ENUM_HPP_
