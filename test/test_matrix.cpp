//
// File       : test_matrix.cpp
// Licence    : see LICENCE
// Maintainer : <your name here>
//
// Tests Matrix class from aline library.
//

#include <limits>       // std::numeric_limits<T>::epsilon()
#include <vector>       // std::vector
#include "unit_test.h"
#include "matrix.h"

using namespace aline;

using uint = unsigned int;
using real = double;
using Vec2i = Vector<int,2ul>;
using Vec2r = Vector<real,2ul>;
using Vec3i = Vector<int,3ul>;
using Vec3r = Vector<real,3ul>;
using Vec4i = Vector<int,4ul>;
using Vec4r = Vector<real,3ul>;
using Mat33i = Matrix<int,3ul,3ul>;
using Mat33r = Matrix<real,3ul,3ul>;
using Mat44i = Matrix<int,4ul,4ul>;
using Mat44r = Matrix<real,4ul,4ul>;

int test_constructors()
{
  int failures { 0 };

  Mat33i a;
  Mat33i b { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };
  Mat33i c = a;
  c[0][1] = 3;
  c[1][1] = 4;
  c[2][1] = 5;
  Mat33i d = { {6} };
  TestVector test_vec
  {
    {"a.at( 0 ) == Vec3i {0,0,0}", a.at( 0 ) == Vec3i {0,0,0} },
    {"a.at( 1 ) == Vec3i {0,0,0}", a.at( 1 ) == Vec3i {0,0,0} },
    {"a.at( 2 ) == Vec3i {0,0,0}", a.at( 2 ) == Vec3i {0,0,0} },
    {"b.at( 0,0 ) == 0", b.at( 0,0 ) == 0 },
    {"b.at( 0,1 ) == 1", b.at( 0,1 ) == 1 },
    {"b.at( 0,2 ) == 2", b.at( 0,2 ) == 2 },
    {"b.at( 1,0 ) == 3", b.at( 1,0 ) == 3 },
    {"b.at( 1,1 ) == 4", b.at( 1,1 ) == 4 },
    {"b.at( 1,2 ) == 5", b.at( 1,2 ) == 5 },
    {"c.at( 0,1 ) == 3", c.at( 0,1 ) == 3 },
    {"c.at( 1,1 ) == 4", c.at( 1,1 ) == 4 },
    {"c.at( 2,1 ) == 5", c.at( 2,1 ) == 5 },
    {"a.at( 0,1 ) == 0", a.at( 0,1 ) == 0 },
    {"a.at( 1,1 ) == 0", a.at( 1,1 ) == 0 },
    {"a.at( 2,1 ) == 0", a.at( 2,1 ) == 0 },
    {"d.at( 0,0 ) == 6", d.at( 0,0 ) == 6 },
    {"d.at( 1,1 ) == 0", d.at( 1,1 ) == 0 },
    {"d.at( 2,2 ) == 0", d.at( 2,2 ) == 0 },
  };

  try
  {
    Vec3i v = d.at( -1 );
    std::cerr << "Failure: d.at( -1 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}
  try
  {
    int v = d.at( 0,-1 );
    std::cerr << "Failure: d.at( 0,-1 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}
  try
  {
    Vec3i v = d.at( 3 );
    std::cerr << "Failure: d.at( 3 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}
  try
  {
    int v = d.at( 0,3 );
    std::cerr << "Failure: d.at( 0,3 ) = " << v << std::endl;
    ++failures;
  }  
  catch( ... ) {}

  return failures + run_tests ("Constructors", test_vec );
}

// It is done just for the form.
// In fact, it has already been testes in the function above.
int test_at()
{
  Mat33i a;
  Mat44i b;

  TestVector test_vec
    { { "a.at( 0 ) == Vec3i {0,0,0}", a.at( 0 ) == Vec3i {0,0,0} }
    , { "b.at( 0 ) == Vec4i {0,0,0}", b.at( 0 ) == Vec4i {0,0,0} } };

  return run_tests( "at()", test_vec );
}

int test_operator_subscript()
{
  std::vector<Mat33r> v
    { { {}, { 0, 0, 1 } }
    , { {}, { .5, .5, .5 } }
    , { {}, {.5*sqrt(2.0) , .5*-sqrt(2.0), 1.0 } } };

    v[0][1] = Vec3r {3,3,3};
    v[1][2] = Vec3r {7,7,7};
    v[2][0] = Vec3r {8,8,8};

    TestVector test_vec
      { {"v[0][1] == Vec3r {3,3,3}", v[0][1] == Vec3r {3,3,3} }
      , {"v[1][2] == Vec3r {7,7,7}", v[1][2] == Vec3r {7,7,7} }
      , {"v[2][0] == Vec3r {8,8,8}", v[2][0] == Vec3r {8,8,8} } };

    return run_tests( "operator[]", test_vec);
}

int test_operator_pluseq()
{
    std::vector<Mat33r> v
      { { {}, { 0.1, 0.0, 0.1 }, {} }
      , { {}, { 0.0, 0.1, 0.0 }, {} }
      , { {}, { 0.2, 0.2, 0.2 }, {} } };

    v[0] += v[1];
    v[0] += v[0];

    TestVector test_vec { { "v[0] == v[2]", v[0] == v[2] } };

    return run_tests( "operator+=( Matrix )", test_vec );
}

int test_isnan()
{
    Mat33i a;
    Mat33i b;
    Mat33r g { { NAN, 0 } };
    Mat33r h { { 0, NAN } };

    TestVector test_vec
    { { "not isnan( a )", not isnan( a ) }
    , { "not isnan( b )", not isnan( b ) }
    , { "isnan( g )" , isnan( g )  }
    , { "isnan( h )" , isnan( h )  } };

    return run_tests( "isnan( Matrix )", test_vec );
}

int test_nearly_equal()
{
  // Machine epsilon (the difference between 1 and the least value greater than
  // 1 that is representable).
  // It is good to compare against 1 but not against very big, or very small
  // (near zero) numbers.
  const float epsilon = std::numeric_limits<float>::epsilon();

  Matrix<float,2ul,2ul> a  { {}, { 1, 2 } };
  Matrix<float,2ul,2ul> b  { {}, { 1.0000001, 2 } };
  Matrix<float,2ul,2ul> b2 { {}, { 1.00000018, 2 } };
  Matrix<float,2ul,2ul> b3 { {}, {1 + epsilon, 2 } };
  Matrix<float,2ul,2ul> b4 { {}, {1 + epsilon + epsilon, 2 } };
  Matrix<float,2ul,2ul> c  { {}, {1.234, 2 } };
  Matrix<float,2ul,2ul> d  { {}, {1.242, 2 } };
  Matrix<float,2ul,2ul> e  { {}, {67329.234, 3 } };
  Matrix<float,2ul,2ul> f  { {}, {67329.242, 3 } };

  TestVector test_vec
  { { "nearly_equal( a, b )",      nearly_equal( a, b ) }
  , { "not nearly_equal( a, b2 )", not nearly_equal( a, b2 ) }
  , { "nearly_equal( a, b3 )",     nearly_equal( a, b3 ) }
  , { "nearly_equal( a, b4 )",     not nearly_equal( a, b4 ) }
  , { "not nearly_equal( c, d )",  not nearly_equal( c, d ) }
  , { "nearly_equal( e, f )",      nearly_equal( e, f ) } };

  return run_tests( "nearly_equal( Matrix, Matrix )", test_vec );
}

int test_operator_output()
{
    std::cout << "Testing operator<<( ostream, Matrix )\n";

    std::vector<Mat33r> v
      { { {}, { 0, 0, 1 }, {} }
      , { {}, { .5, .5, .5 }, {} }
      , { {}, { .5*sqrt(2.0), .5*-sqrt(2.0), 1.0 }, {} } };

    std::cout <<
        v[0] << std::endl <<
        v[1] << std::endl <<
        v[2] << std::endl;

    Mat44r a { { 1.0, 3.5,  1.2, 4.4 } };
    Mat44r b { {}, { 1.0, 2.0,  3.0, 4.0 } };
    Mat44r c { {}, {}, { 0.0, 1.5, -1.8, 0.4 } };

    std::cout <<
        a << std::endl <<
        b << std::endl <<
        c << std::endl;

    Mat33r m
    {
        { 0, 0, 1 },
        { .5, .5, .5 },
        { .5*sqrt(2.0), .5*-sqrt(2.0), 1.0 }
    };
    std::cout << m << std::endl;
 
    std::cout << std::endl;

    return 0;
}

int test_operator_plus()
{
    Mat33r m1
      { { 0.0, 0.0, 0.0 }
      , { 0.0, 0.0, 0.0 }
      , { 0.0, 0.0, 0.0 } };
    Mat33r m2
      { { 3.0, 0.0, 1.0 }
      , { 3.1, 0.0, 1.1 }
      , { 0.5, 0.5, 0.5 } };
    Mat33r m3
      { { 3.0, 0.0, 1.0 }
      , { 3.1, 0.0, 1.1 }
      , { 0.5, 0.5, 0.5 } };

    TestVector test_vec { { "m1 + m2 == m3", m1 + m2 == m3 } };

    return run_tests( "operator+( Matrix, Matrix )", test_vec );
}

int test_operator_mult1()
{
  Matrix<int,3,2> a
    { {1,2}
    , {3,4}
    , {5,6} };
  Matrix<int,2,4> b
    { {1,2,3,4}
    , {5,6,7,8} };
  Matrix<int,3,4> c
    { {1*1 + 2*5, 1*2 + 2*6, 1*3 + 2*7, 1*4 + 2*8}
    , {3*1 + 4*5, 3*2 + 4*6, 3*3 + 4*7, 3*4 + 4*8}
    , {5*1 + 6*5, 5*2 + 6*6, 5*3 + 6*7, 5*4 + 6*8} };

  TestVector test_vec { { "a * b == c", a * b == c } };

  return run_tests( "operator*( Matrix, Matrix )", test_vec );
}

int test_operator_mult2()
{
  Matrix<int,3,2> a
    { {1,2}
    , {3,4}
    , {5,6} };
  Vector<int,2> b {1,2};
  Vector<int,3> c {1*1 + 2*2, 3*1 + 4*2, 5*1 + 6*2};

  TestVector test_vec { { "a * b == c", a * b == c } };

  return run_tests( "operator*( Matrix, Vector )", test_vec );
}

int test_operator_mult3()
{
  Matrix<int,3,2> a
    { {1,2}
    , {3,4}
    , {5,6} };
  int b { 3 };
  Matrix<int,3,2> c
    { {b * a[0][0], b * a[0][1]}
    , {b * a[1][0], b * a[1][1]}
    , {b * a[2][0], b * a[2][1]} };

  TestVector test_vec { { "a * b == c", a * b == c } };

  return run_tests( "operator*( Matrix, Scalar )", test_vec );
}

int test_operator_div()
{
  Matrix<int,3,2> a
    { {1,2}
    , {3,4}
    , {5,6} };
  int b { 3 };
  Matrix<int,3,2> c
    { {1/b * a[0][0], 1/b * a[0][1]}
    , {1/b * a[1][0], 1/b * a[1][1]}
    , {1/b * a[2][0], 1/b * a[2][1]} };

  TestVector test_vec { { "a / b == c", a / b == c } };

  return run_tests( "operator*( Matrix, Scalar )", test_vec );
}

int test_operator_negation()
{
    Matrix<int,2,3> a
     { {  1,  2,  3 }
     , { -1, -2, -3 } };
    Matrix<int,2,3> b
     { { -1, -2, -3 }
     , {  1,  2,  3 } };

    TestVector test_vec { { "-a == b", -a == b} };

    return run_tests( "operator-( Matrix )", test_vec );
}

int test_operator_minus()
{
    Mat44r a { {}, {1.0, 3.5, 1.2, 4.4} };
    Mat44r b { {}, {1.0, 2.0, 3.0, 4.0} };
    Mat44r c { {}, { a[1][0]-b[1][0]
                   , a[1][1]-b[1][1]
                   , a[1][2]-b[1][2]
                   , a[1][3]-b[1][3] } };

    TestVector test_vec { { "a - b == c", a - b == c } };

    return run_tests( "operator-( Matrix, Matrix )", test_vec );
}

int test_to_string()
{
    Mat33r m
    {
        { 0, 0, 1 },
        { .5, .5, .5 },
        { .5*sqrt(2.0), .5*-sqrt(2.0), 1.0 }
    };
    std::string s3 = to_string( m );
 
    std::string s = "s3 == ((0.000000, 0.000000, 1.000000)";
    s+= ", (0.500000, 0.500000, 0.500000)";
    s+= ", (0.707107, 0.707107, 1.000000))";
    TestVector test_vec
      { { s
        , s3 == "((0.000000, 0.000000, 1.000000), (0.500000, 0.500000, 0.500000), (0.707107, -0.707107, 1.000000))" } };
  
    return run_tests( "to_string( Matrix )", test_vec);
}

int test_transpose()
{
  Mat33i a { {1,2,3},{4,5,6},{7,8,9} };
  Mat33i b { {1,4,7},{2,5,8},{3,6,9} };

  TestVector test_vec { { "transpose( a ) == b", transpose( a ) == b } };

  return run_tests( "transpose( Matrix )", test_vec );
}

int main()
{
    int failures { 0 };
    /*
    failures += test_constructors();
    failures += test_at();
    failures += test_operator_subscript();
    failures += test_operator_pluseq();

    failures += test_isnan();
    failures += test_nearly_equal();
    failures += test_operator_plus();
    failures += test_operator_negation();
    failures += test_operator_minus();
    failures += test_operator_mult1();
    failures += test_operator_mult2();
    failures += test_operator_mult3();
    failures += test_operator_div();
    failures += test_to_string();
    failures += test_transpose();

    failures += test_operator_output();
    */

    Matrix<double, 3, 3> m = {
        {2, 3, 8},
        {6, 0, -3},
        {-1, 3, 2}
    };

    Matrix<double, 3, 3> inv = inverse(m);

    std::cout << m << std::endl;
    std::cout << inv << std::endl;

    if( failures > 0 )
    {
        std::cout << "Total failures : " << failures << std::endl;
        std::cout << "THE TEST FAILED!!" << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Success!" << std::endl;
        return 0;
    }
}
