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

TEST_CASE( "Test conversion to names" )
{
  NAMED_SCOPED_ENUM(Color, red, green, blue );
  
  Color color = Color::red;
  
  CHECK( color.name() == "red" );
  
}

TEST_CASE( "Test set function" )
{
  NAMED_SCOPED_ENUM(Color, red, green, blue );
  
  Color color = Color::red;
  
  color = Color::green;
  
  CHECK( color == Color::green );
}

TEST_CASE( "Test size function" )
{
  NAMED_SCOPED_ENUM(Color, red, green, blue );
  
  CHECK( Color::size() == 3 );
}
