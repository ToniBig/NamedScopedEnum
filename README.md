[![Version](https://img.shields.io/badge/c%2B%2B-14-blue.svg)](http://en.cppreference.com/w/cpp)
[![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://en.wikipedia.org/wiki/MIT_License)
[![Build Status](https://travis-ci.org/ToniBig/NamedScopedEnum.svg?branch=master)](https://travis-ci.org/ToniBig/NamedScopedEnum)
[![Build status](https://ci.appveyor.com/api/projects/status/3ybtwektk15wwpfu?svg=true)](https://ci.appveyor.com/project/ToniBig/namedscopedenum)
[![Coverage Status](https://coveralls.io/repos/github/ToniBig/NamedScopedEnum/badge.svg?branch=master)](https://coveralls.io/github/ToniBig/NamedScopedEnum?branch=master)

# Named Enums
This library provides the following macros to __create__ standard __enums__ and __enum classes__ with __stringified enumerators__

* `MAKE_NAMED_ENUM(enum_name,...)` creates a standard enum "enum_name" with named enumerators "..."
* `MAKE_NAMED_ENUM_CLASS(enum_name,...)` creates a C++11 enum class "enum_name" with named enumerators "..."
* `MAKE_NAMED_ENUM_WITH_TYPE(enum_name,enum_type,...)` creates a standard enum "enum_name" with named enumerators "..." and underlying type "enum_type"
* `MAKE_NAMED_ENUM_CLASS_WITH_TYPE(enum_name,enum_type,...)` creates a C++11 enum class "enum_name" with named enumerators "..." and underlying type "enum_type"

## A short example

```cpp
#include "named_enum.hpp"
#include <iostream>

MAKE_NAMED_ENUM_CLASS(FirstThree, One, Two, Three);

int main( int argc, char **argv ){
  for ( const auto & name : named_enum::names<FirstThree>() )
  {
    std::cout << name << "\n";
  }
}
```
Output:
```
One
Two
Three
```


## Features
* __Single-header-only__ library
* The __number of enumerators__, i.e. the size, is available
* __All__ enumerator __names__ are available as std::array of std::strings
* The __current name__ is available as std::string
* All __string__ representations are created at __compile time__

## Limitation
* The macro does not support custom values for the enumerators

## Dependencies (standard library)
* array
* string
* type_traits

## Synopsis
When an enum `E` is created with any the four macros, the specialized versions of the functions 
below are created.

```cpp
// __Size interface__
template<typename E>
constexpr auto size -> unsigned int ( );

template<typename E>
constexpr auto size -> unsigned int ( E const & e ) {
  return size<E>();
}

// __Name interface__
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
```

## Simple usage

```cpp
#include "named_enum.hpp"
#include <iostream>

MAKE_NAMED_ENUM_CLASS( Colors, GREEN, YELLOW, RED );

class TrafficLight {
public:
  TrafficLight( Colors color ) :
          color_( color ){
    std::cout << "Initialized signal with: " << get_name( ) << std::endl;
  }

  void set_color( Colors color ){
    color_ = color;
    std::cout << "Set signal to: " << get_name( ) << std::endl;
  }

  Colors get_color( ) const{
    return color_;
  }

  std::string get_name( ) const{
    return named_enum::name( color_ );
  }

private:
  Colors color_;
};

int main( int argc, char **argv ){
  TrafficLight signal( Colors::RED );

  signal.set_color( Colors::YELLOW );

  signal.set_color( Colors::GREEN );

  auto current_color = signal.get_name( );

  std::cout << "current_color: " << current_color << std::endl;

  std::cout << "Available colors are:";

  for ( const auto & color : named_enum::names<Colors>( ) ) {
    std::cout << " " << color;
  }

  std::cout << std::endl;
}
```

Ouput:
```
Initialized signal with: RED
Set signal to: YELLOW
Set signal to: GREEN
current_color: GREEN
Available colors are: GREEN YELLOW RED
```

## Create a static map using a std::array

To create a map with a fixed number of named keys and constant time look up
just combine a `std::array` with the `MAKED_NAMED_ENUM` macro, whose enumerators 
automatically convert to interal types. 

```cpp
#include "../named_enum.hpp"
#include <array>
#include <iostream>

// Something healthy
MAKE_NAMED_ENUM( FRUIT, APPLE, ORANGE, BANANA, PLUM );

int main( int argc, char **argv )
{
  static const size_t size=named_enum::size<FRUIT>( );
  std::array<int, size> fruitMap; // size() is constexpr

  fruitMap[FRUIT::APPLE] = 4;
  fruitMap[FRUIT::ORANGE] = 1;
  fruitMap[FRUIT::BANANA] = 3;
  fruitMap[FRUIT::PLUM] = 2;

  std::cout << "How much fruit is in the basket?" << std::endl;

  for ( size_t i = 0; i < size; ++i )
  {
    auto names = named_enum::names<FRUIT>( ); // names() is static
    std::cout << names[i] << " : " << fruitMap[i] << std::endl;
  } // end of i -loop
}
```
Output:
```
How much fruit is in the basket?
APPLE : 4
ORANGE : 1
BANANA : 3
PLUM : 2
```
