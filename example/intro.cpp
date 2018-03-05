#include "../named_enum.hpp"
#include <iostream>

MAKE_NAMED_ENUM_CLASS(FirstThree, One, Two, Three);

int main( int argc, char **argv ){
  for ( const auto & name : named_enum::names<FirstThree>() )
  {
    std::cout << name << "\n";
  }
}
