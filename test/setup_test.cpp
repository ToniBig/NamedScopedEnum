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

// --- Internal Includes ---
#include "../named_scoped_enum.hpp"
#include "catch.hpp"

TEST_CASE("Empty enumerator fails"){
//  NAMED_SCOPED_ENUM(Foo); <-- error: static assertion failed: No enumerators provided
}

TEST_CASE("Custom enumerators fail"){
//  NAMED_SCOPED_ENUM(Foo,
//                    A = 1,
//                    B,
//                    C); <-- error: static assertion failed: Custom enumerators (=) are not supported
}

NAMED_SCOPED_ENUM( Color, red, green, blue );

TEST_CASE( "Size function is constexpr" ){

  static_assert(Color::size()==3,"Incorrect size");

  CHECK( Color::size( ) == 3 );
}

NAMED_SCOPED_ENUM( Foo, A, B, C );
NAMED_SCOPED_ENUM( Bar, A, B, C );

TEST_CASE("Comparing two different enums fails"){

  Foo foo { Foo::A };
  Bar bar { Bar::C };

//  CHECK( foo != bar ); // <--  error: no match for ‘operator!=’
//  CHECK( foo < bar );  // <--  error: no match for ‘operator<’
//  CHECK( bar > foo );  // <--  error: no match for ‘operator>’
//  CHECK( foo <= bar ); // <--  error: no match for ‘operator<=’
//  CHECK( bar >= foo ); // <--  error: no match for ‘operator>=’
}

TEST_CASE("Assigning two different enums fails"){

  Foo foo { Foo::A };
  Bar bar { Bar::B };

//  foo = bar; --> compiler error
}

TEST_CASE( "Enumerators have names" ){
  auto color = Color { };

  color = Color::red;
  CHECK( color.name( ) == "red" );
  color = Color::green;
  CHECK( color.name( ) == "green" );
  color = Color::blue;
  CHECK( color.name( ) == "blue" );
}

TEST_CASE("Enum has many names"){
  std::vector<std::string> names=Color::names();

  REQUIRE(names.size()==3);
  CHECK(names[0]=="red");
  CHECK(names[1]=="green");
  CHECK(names[2]=="blue");
}

TEST_CASE("Enums of same type can be compared"){
  Color A = Color::red;
  Color B = Color::red;
  Color C = Color::blue;

  CHECK(A==B);
  CHECK(A!=C);
  CHECK(A<C);
  CHECK(A<=C);
  CHECK(C>A);
  CHECK(C>=A);
}
