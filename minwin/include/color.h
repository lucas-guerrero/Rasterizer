//!
//! \brief     Declares class Color and related elements.
//!
//! \file      color.h
//! \copyright (c) Université d'Artois 2021
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _COLOR_H_
#define _COLOR_H_

#include <sstream>
//#include <SDL2/SDL.h>
// Uncomment the line above if the one below does not work.
#include "SDL2/SDL.h"

namespace minwin {

class Color 
{
  public:
    double red;
    double green;
    double blue;
    double alpha;

};

bool operator==( const Color &, const Color &);
std::string to_string( const Color & );

// Some basic color names
static constexpr Color black        { 0.0,  0.0,  0.0,  0.0 };
static constexpr Color white        { 1.0,  1.0,  1.0,  0.0 };
static constexpr Color red          { 1.0,  0.0,  0.0,  0.0 };
static constexpr Color green        { 0.0,  1.0,  0.0,  0.0 };
static constexpr Color blue         { 0.0,  0.0,  1.0,  0.0 };
static constexpr Color darkgrey     { 0.86, 0.86, 0.86, 0.0 };
static constexpr Color grey         { 0.5,  0.5,  0.5,  0.0 };
static constexpr Color cyan         { 0.0,  1.0,  1.0,  0.0 };
static constexpr Color yellow       { 1.0,  1.0,  0.0,  0.0 };
static constexpr Color magenta      { 1.0,  0.0,  1.0,  0.0 };

} // end of namespace minwin

#endif // _COLOR_H_

