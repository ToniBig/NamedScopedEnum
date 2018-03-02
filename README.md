# Named Scoped Enum
Create scoped __enum__ providing __number__ and __string descriptors__ of all enumerators

[![Version](https://img.shields.io/badge/c%2B%2B-14-blue.svg)](http://en.cppreference.com/w/cpp)
[![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)](https://en.wikipedia.org/wiki/MIT_License)
[![Build Status](https://travis-ci.org/ToniBig/NamedScopedEnum.svg?branch=master)](https://travis-ci.org/ToniBig/NamedScopedEnum)
[![Build status](https://ci.appveyor.com/api/projects/status/3ybtwektk15wwpfu?svg=true)](https://ci.appveyor.com/project/ToniBig/namedscopedenum)

## Features
* __Single-header-only__ library providing the macro __`NAMED_SCOPED_ENUM`__
* The macro creates enums, which are __scoped__ like C++11's enum classes __but convert to integral types automatically__
* The __number of enumerators__, i.e. the size, is available via the __constexpr__ member function __`size()`__
* __All__ enumerator __names__ are available `std::vector<std::string>` via __static__ member function __`names()`__
* The __current name__ is available as std::string via member the functions __`operator std::string`__ and __`name()`__
* __String__ representations of __all enumerators__ are created at __compile time__

## Limitation
* The macro does not support custom values for the enumerators

## Dependencies
* std::vector
* std::string

## Synopsis
To create an scoped enumeration, provide a name and enumerators for the enum to be created to the `NAMED_SCOPED_ENUM` macro. The created type provides the enumerators, the size of the enum and the individual names (std::string representations) of the enumerators. 

The following call to the macro `NAMED_SCOPED_ENUM`
```cpp
NAMED_SCOPED_ENUM( Colors, GREEN, YELLOW, RED );
```
will generate a class like this

```cpp
struct Colors 
{
public:
  ENUM { GREEN, YELLOW, RED};
  
  constexpr Colors(ENUM e = ENUM{});
  
  static constexpr size_t size();
  
  static std::string name(size_t i);
  static std::vector<std::string> const & names();
  
  std::string name() const;
  operator std::string() const;
};
```

## Simple usage

```cpp
#include "named_scoped_enum.hpp"

NAMED_SCOPED_ENUM( Colors, GREEN, YELLOW, RED );

class TrafficLight
{
public:
  TrafficLight( Colors color ) :
          color_( color )
  {
    std::cout << "Initialized signal with: " << color_.name( ) << std::endl;
  }

  void set_color( Colors color )
  {
    color_ = color;
    std::cout << "Set signal to: " << color_.name( ) << std::endl;
  }

  Colors get_color( ) const
  {
    return color_;
  }

private:
  Colors color_;
};

int main( int argc,
          char **argv )
{
  TrafficLight signal( Colors::RED );

  signal.set_color( Colors::YELLOW );

  signal.set_color( Colors::GREEN );

  std::string current_color = signal.get_color( );

  std::cout << "current_color: " << current_color << std::endl;

  std::cout << "Available colors are:";

  for ( const auto & color : Colors::names( ) )
  {
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
just combine a `std::array` with a `NAMED_SCOPED_ENUM`. 

```cpp
#include "named_scoped_enum.hpp"

#include <array>

int main( int argc,
          char **argv )
{
  NAMED_SCOPED_ENUM( FRUIT, APPLE, ORANGE, BANANA, PLUM ); // Something healthy

  std::array<int, FRUIT::size( )> fruitMap;                // size() is constexpr

  fruitMap[FRUIT::APPLE] = 4;
  fruitMap[FRUIT::ORANGE] = 1;
  fruitMap[FRUIT::BANANA] = 3;
  fruitMap[FRUIT::PLUM] = 2;

  std::cout << "How much fruit is in the basket?" << std::endl;

  for ( size_t i = 0; i < FRUIT::size( ); ++i )
  {
    auto names = FRUIT::names( );                          // names() is static
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
