//!
//! \brief     Defines class Color related functions.
//!
//! \file      color.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "color.h"

namespace minwin {

//! A string representation of the color.
std::string to_string( const Color & color )
{
	return std::to_string( color.red ) + ", "
       + std::to_string( color.green ) + ", "
       + std::to_string( color.blue ) + ", "
       + std::to_string( color.alpha );
}

//! Equality test.
bool operator==( const Color & color1, const Color & color2 )
{
  return color1.red   == color2.red
     and color1.green == color2.green
     and color1.blue  == color2.blue
     and color1.alpha == color2.alpha;
}

} // end of namespace minwin

