//
//
// File       : test_vector.cpp
// Licence    : see LICENCE
// Maintainer : <your name here>
//
// Tests Vector class from aline library.
//

#include <limits>       // std::numeric_limits<T>::epsilon()
#include <iostream>     // std::cout
#include <vector>       // std::vector
#include "unit_test.h"
#include "vector.h"

using namespace aline;

using uint = unsigned int;
using real = double;
using Vec2i = Vector<int,2ul>;
using Vec2r = Vector<real,3ul>;
using Vec3i = Vector<int,3ul>;
using Vec3r = Vector<real,3ul>;
using Vec4i = Vector<int,4ul>;
using Vec4r = Vector<real,4ul>;

int test_constructors()
{
  int failures { 0 };

  Vec3i a;
  Vec3i b { 0, 1, 2 };
  Vec3i c = a;
  c[0] = 3;
  c[1] = 4;
  c[2] = 5;
  Vec3i d = { 6 };
  TestVector test_vec
  {
    {"a.at( 0 ) == 0", a.at( 0 ) == 0 },
    {"a.at( 1 ) == 0", a.at( 1 ) == 0 },
    {"a.at( 2 ) == 0", a.at( 2 ) == 0 },
    {"b.at( 0 ) == 0", b.at( 0 ) == 0 },
    {"b.at( 1 ) == 1", b.at( 1 ) == 1 },
    {"b.at( 2 ) == 2", b.at( 2 ) == 2 },
    {"c.at( 0 ) == 3", c.at( 0 ) == 3 },
    {"c.at( 1 ) == 4", c.at( 1 ) == 4 },
    {"c.at( 2 ) == 5", c.at( 2 ) == 5 },
    {"a.at( 0 ) == 0", a.at( 0 ) == 0 },
    {"a.at( 1 ) == 0", a.at( 1 ) == 0 },
    {"a.at( 2 ) == 0", a.at( 2 ) == 0 },
    {"d.at( 0 ) == 6", d.at( 0 ) == 6 },
    {"d.at( 1 ) == 0", d.at( 1 ) == 0 },
    {"d.at( 2 ) == 0", d.at( 2 ) == 0 },
  };

  try
  {
    int v = d.at( -1 );
    std::cout << "Failure: d.at( -1 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}
  try
  {
    int v = d.at( 3 );
    std::cout << "Failure: d.at( 3 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}

  return failures + run_tests ("Constructors", test_vec );
}

int test_at()
{
  Vec3i a;
  Vec4i b;
  TestVector test_vec
  {
    { "a.at( 0 ) == 0", a.at( 0 ) == 0 },
    { "b.at( 0 ) == 0", b.at( 0 ) == 0 },
  };
  return run_tests( "at()", test_vec );
}

int test_isnan()
{
    Vec2i a;
    Vec3i b;
    Vec2r g { NAN, 0 };
    Vec2r h { 0, NAN };

    TestVector test_vec
    {
        { "not isnan( a )", not isnan( a ) },
        { "not isnan( b )", not isnan( b ) },
        { "isnan( g )" , isnan( g )  },
        { "isnan( h )" , isnan( h )  },
    };

    return run_tests( "isnan( Vector )", test_vec );
}

int test_is_unit()
{
  std::vector<Vec3r> t
  { 
    { 0, 0, 1 },
    { 0, 1, 0 },
    { 1, 0, 0 },
    { .5*sqrt(2.0), .5*sqrt(2.0), 0 },
  };
    
  TestVector test_vec;
  for( size_t i { 0 }; i < t.size(); ++i )
  {
    test_vec.push_back( { "is_unit( t[" + std::to_string( i ) + "] )"
                        , is_unit( t[i] ) } );
  }
   
  return run_tests( "is_unit( Vector )", test_vec );
}

int test_nearly_equal()
{
  // Machine epsilon (the difference between 1 and the least value greater than
  // 1 that is representable).
  // It is good to compare against 1 but not against very big, or very small
  // (near zero) numbers.
  const float epsilon = std::numeric_limits<float>::epsilon();

  Vector<float,2ul> a  { 1, 2 };
  Vector<float,2ul> b  { 1.0000001, 2 };
  Vector<float,2ul> b2 { 1.00000018, 2 };
  Vector<float,2ul> b3 { 1 + epsilon, 2 };
  Vector<float,2ul> b4 { 1 + epsilon + epsilon, 2 };
  Vector<float,2ul> c  { 1.234, 2 };
  Vector<float,2ul> d  { 1.242, 2 };
  Vector<float,2ul> e  { 67329.234, 3 };
  Vector<float,2ul> f  { 67329.242, 3 };

  TestVector test_vec
  { { "nearly_equal( a, b )",      nearly_equal( a, b ) }
  , { "not nearly_equal( a, b2 )", not nearly_equal( a, b2 ) }
  , { "nearly_equal( a, b3 )",     nearly_equal( a, b3 ) }
  , { "nearly_equal( a, b4 )",     not nearly_equal( a, b4 ) }
  , { "not nearly_equal( c, d )",  not nearly_equal( c, d ) }
  , { "nearly_equal( e, f )",      nearly_equal( e, f ) } };

  return run_tests( "nearly_equal( Vector, Vector )", test_vec );
}

int test_norm()
{
  std::vector<Vec3r> v
  {
    { 0, 1, 2 },
    { 1, 2, 3 },
  };

  TestVector test_vec
  {
    { "norm( " + to_string( v[0] ) + " == sqrt( 5.0 )" , norm( v[0] ) == sqrt( 5.0 ) },
    { "norm( " + to_string( v[1] ) + " == sqrt( 14.0 )", norm( v[1] ) == sqrt( 14.0 ) },
  };

  return run_tests( "norm( Vector )", test_vec );
}

int test_operator_subscript()
{
  std::vector<Vec3r> v { { 0, 0, 1 }
                       , { .5, .5, .5 }
                       , { .5*sqrt(2.0) , .5*-sqrt(2.0), 1.0 } };

    v[0][1] = 3;
    v[1][2] = 7;
    v[2][0] = 8;

    TestVector test_vec
    {
        {"v[0][1] == 3", v[0][1] == 3},
        {"v[1][2] == 7", v[1][2] == 7},
        {"v[2][0] == 8", v[2][0] == 8},
    };

    return run_tests( "operator[]", test_vec);
}

int test_operator_output()
{
    std::cout << "Testing operator<<( ostream, Vector )\n";

    std::vector<Vec3r> v
    {
        { 0, 0, 1 },
        { .5, .5, .5 },
        { .5*sqrt(2.0), .5*-sqrt(2.0), 1.0 }
    };
    std::cout <<
        v[0] << std::endl <<
        v[1] << std::endl <<
        v[2] << std::endl;
    Vec4r a { 1.0, 3.5,  1.2, 4.4 };
    Vec4r b { 1.0, 2.0,  3.0, 4.0 };
    Vec4r c { 0.0, 1.5, -1.8, 0.4 };
    std::cout <<
        a << std::endl <<
        b << std::endl <<
        c << std::endl;

    std::cout << std::endl;

    return 0;
}

int test_operator_plus()
{
    std::vector<Vec3r> v
    {
        { 0.0, 0.0, 0.0 },
        { 0.1, 0.0, 0.1 },
        { 0.2, 0.0, 0.2 },
    };

    TestVector test_vec
    {
        { "v[0] + v[1] == v[1]", v[0] + v[1] == v[1] },
        { "v[0] + v[2] == v[2]", v[0] + v[2] == v[2] },
        { "v[1] + v[1] == v[2]", v[1] + v[1] == v[2] },
    };

    return run_tests( "operator+( Vector )", test_vec );
}

int test_operator_pluseq()
{
    std::vector<Vec3r> v
      { { 0.1, 0.0, 0.1 }
      , { 0.0, 0.1, 0.0 }
      , { 0.2, 0.2, 0.2 } };

    v[0] += v[1];
    v[0] += v[0];

    TestVector test_vec { { "v[0] == v[2]", v[0] == v[2] } };

    return run_tests( "operator+=( Vector )", test_vec );
}

int test_operator_negation()
{
    std::vector<Vec3r> v
    {
        {  1,  2,  3 },
        { -1, -2, -3 },
    };
    TestVector test_vec
    {
        { "-v[0] == v[1]", -v[0] == v[1] },
        { "-v[1] == v[0]", -v[1] == v[0] },
    };

    return run_tests( "operator-( Vector )", test_vec );
}

int test_operator_minus()
{
    Vec4r a { 1.0, 3.5,  1.2, 4.4 };
    Vec4r b { 1.0, 2.0,  3.0, 4.0 };
    Vec4r c { 0.0, 1.5, -1.8, (4.4 - 4.0) };

    TestVector test_vec { { "a - b == c", a - b == c } };

    return run_tests( "operator-( Vector, Vector )", test_vec );
}

int test_dot()
{
    Vec4r a { 1.0, 3.0, 1.0, 4.0 };
    Vec4r b { 2.0, 7.0, 2.4, 8.8 };
    real  x { 60.6 };
    TestVector test_vec
    {
        { "dot( a, b ) == x", dot( a, b ) == x },
    };

    return run_tests( "dot( Vector, Vector )", test_vec );
 }

int test_cross()
{
	Vec4r v0 { 0, 1, 2, 3 };
	Vec4r v1 { 3, 4, 5, 6 };
	Vec4r v2 { 5 - 8, 6 - 0, 0 - 3, 0 }; 
    TestVector test_vec
    {
        { "cross( v0, v1 ) == v2", cross( v0, v1 ) == v2 },
    };

    return run_tests( "cross( Vector, Vector )", test_vec );
}

int test_unit_vector()
{
  Vec3r v1 { 0.0, 0.0, 0.0 };
  Vec3r v2 { 1.0, 1.0, 0.0 };
  Vec3r v3 { 1.0, 0.0, 0.0 };
  Vec3r v4 { 2.0, 3.0, 1.0 };

  TestVector test_vec
  { { "isnan( unit_vector( v1 ) ) == true"  , isnan( unit_vector( v1 ) )   }
  , { "is_unit( unit_vector( v2 ) ) == true", is_unit( unit_vector( v2 ) ) }
  , { "is_unit( unit_vector( v3 ) ) == true", is_unit( unit_vector( v3 ) ) }
  , { "is_unit( unit_vector( v4 ) ) == true", is_unit( unit_vector( v4 ) ) } };

  return run_tests( "unit_vector( Vector )", test_vec );
}

int main()
{
    int failures { 0 };

    failures += test_constructors();
    failures += test_at();
    failures += test_operator_subscript();
    failures += test_operator_pluseq();

    failures += test_cross();
    failures += test_dot();
    failures += test_isnan();
    failures += test_is_unit();
    failures += test_nearly_equal();
    failures += test_norm();
    failures += test_operator_plus();
    failures += test_operator_negation();
    failures += test_operator_minus();
    failures += test_unit_vector();

    failures += test_operator_output();

    if( failures > 0 )
    {
        std::cout << "Total failures : " << failures << std::endl;
        std::cout << "TEST FAILED!!" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Success!" << std::endl;
        return 0;
    }
}
