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

#include "../named_enum.hpp"

#include "catch.hpp"

#include <iostream>

/// TEST_CASE( "Empty enumerator fails" )
//MAKE_NAMED_ENUM(Foo1); // <-- error: static assertion failed: No enumerators provided
//MAKE_NAMED_ENUM_CLASS(Foo2);
//MAKE_NAMED_ENUM_WITH_TYPE(Foo3,int);
//MAKE_NAMED_ENUM_CLASS_WITH_TYPE(Foo4,int);

//TEST_CASE("Custom enumerators fail")
//MAKE_NAMED_ENUM(Bar1,
//                A = 1,
//                B,
//                C); //<-- error: static assertion failed: Custom enumerators (=) are not supported
//MAKE_NAMED_ENUM_CLASS(Bar2,
//                A = 1,
//                B,
//                C); //<-- error: static assertion failed: Custom enumerators (=) are not supported
//MAKE_NAMED_ENUM_WITH_TYPE(Bar3,int,
//                A = 1,
//                B,
//                C); //<-- error: static assertion failed: Custom enumerators (=) are not supported
//MAKE_NAMED_ENUM_CLASS_WITH_TYPE(Bar4,int,
//                A = 1,
//                B,
//                C); //<-- error: static assertion failed: Custom enumerators (=) are not supported

MAKE_NAMED_ENUM( Baz1, A1, B1, C1 );
MAKE_NAMED_ENUM_CLASS( Baz2, A2, B2, C2 );
MAKE_NAMED_ENUM_WITH_TYPE( Baz3, int, A3, B3, C3 );
MAKE_NAMED_ENUM_CLASS_WITH_TYPE( Baz4, int, A4, B4, C4 );

TEST_CASE( "Size function is constexpr" ){

  static_assert(named_enum::size<Baz1>()==3,"Incorrect size");
  static_assert(named_enum::size<Baz2>()==3,"Incorrect size");
  static_assert(named_enum::size<Baz3>()==3,"Incorrect size");
  static_assert(named_enum::size<Baz4>()==3,"Incorrect size");

  CHECK( named_enum::size<Baz1>( ) == 3 );
  CHECK( named_enum::size<Baz2>( ) == 3 );
  CHECK( named_enum::size<Baz3>( ) == 3 );
  CHECK( named_enum::size<Baz4>( ) == 3 );
}

MAKE_NAMED_ENUM_CLASS( Color, red,   green, blue );

bool same(const char * lhs, const char * rhs){
  return std::string(lhs) == std::string(rhs);
}

TEST_CASE( "Enumerators have names" ){
  auto color = Color { };

  color = Color::red;
  CHECK( same(named_enum::name( color ),"red") );
  color = Color::green;
  CHECK( same(named_enum::name( color ), "green") );
  color = Color::blue;
  CHECK( same(named_enum::name( color ), "blue") );
}

TEST_CASE("Enum has many names"){
  auto names = named_enum::names<Color>( );

  REQUIRE( names.size( ) == 3 );

  CHECK( same(names[0], "red") );
  CHECK( same(names[1], "green") );
  CHECK( same(names[2], "blue") );
}
