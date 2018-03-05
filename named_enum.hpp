#include <array>
#include <string>

namespace named_enum {

// Size interface
template<typename E>
constexpr unsigned int size( );

template<typename E>
constexpr unsigned int size( E const & e ) {
  return size<E>();
}

// Name interface
using string_t=std::string;

template<unsigned int N>
using string_array_t=std::array<string_t, N>;

template<typename E>
auto names( ) -> string_array_t<size<E>()> const &;

template<typename E>
auto names( E const & ) -> string_array_t<size<E>()> const & {
  return names<E>( );
}

template<typename E>
auto name( E const & e ) -> string_t {
  return names( e )[static_cast<unsigned int>(e)];
}

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

template<int N>
constexpr bool empty( char const (&string)[N] ){
  return N==1;
}

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

  constexpr operator bool( ) const{
    return true;
  }

  string_array_t<C> strings( ) const{
    auto strings=string_array_t<C>{};
    for (size_t i = 0; i < C; ++i) {
      strings[i]=string_t{&string_[ids_[i]]};
    } // end of i-loop
    return strings;
  }
};

template<size_t C, size_t N> constexpr
tokenizer<N, C> make_tokenizer( char const (&string)[N] ){
  return tokenizer<N, C>( string );
}

} // namespace detail
} // namespace named_enum

#define _MAKE_NAMED_ENUM_WITH_TYPE_IMPL(enum_name,enum_strictness,enum_type,...)       \
                                                                                       \
static_assert(!named_enum::detail::empty(#__VA_ARGS__),"No enumerators provided");     \
                                                                                       \
static_assert(named_enum::detail::count_character(#__VA_ARGS__,'=')==0,                \
  "Custom enumerators (=) are not supported");                                         \
enum enum_strictness enum_name : enum_type {                                           \
  __VA_ARGS__                                                                          \
};                                                                                     \
                                                                                       \
namespace named_enum {                                                                 \
                                                                                       \
template<>                                                                             \
constexpr unsigned int size<enum_name>( ){                                             \
  return detail::count_character(#__VA_ARGS__,',')+1;                                  \
}                                                                                      \
                                                                                       \
template<>                                                                             \
auto names<enum_name>( ) -> string_array_t<size<enum_name>()> const &{                 \
  static auto names=detail::make_tokenizer<size<enum_name>()>(#__VA_ARGS__).strings(); \
  return names;                                                                        \
}                                                                                      \
                                                                                       \
} // namespace named_enum

#define MAKE_NAMED_ENUM(enum_name,...)                                                 \
    _MAKE_NAMED_ENUM_WITH_TYPE_IMPL(enum_name,,int,__VA_ARGS__)

#define MAKE_NAMED_ENUM_CLASS(enum_name,...)                                           \
    _MAKE_NAMED_ENUM_WITH_TYPE_IMPL(enum_name,class,int,__VA_ARGS__)

#define MAKE_NAMED_ENUM_WITH_TYPE(enum_name,enum_type,...)                             \
    _MAKE_NAMED_ENUM_WITH_TYPE_IMPL(enum_name,,enum_type,__VA_ARGS__)

#define MAKE_NAMED_ENUM_CLASS_WITH_TYPE(enum_name,enum_type,...)                       \
    _MAKE_NAMED_ENUM_WITH_TYPE_IMPL(enum_name,class,enum_type,__VA_ARGS__)
