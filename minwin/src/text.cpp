//!
//! \brief Defines class Text functions.
//!
//! \file      text.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "text.h"

using namespace minwin; 

//! Default constructor.
Text::Text() {}

//!  Constructor.
//
//! \param x,y   text position
//! \param str   text contents
//! \param color text color
//
Text::Text( unsigned int x, unsigned int y
          , const std::string & str
          , const Color & color
          )
  : posX   { x }
  , posY   { y }
  , string { str }
  , color  { color }
{}

//! Gets the vertical position of the text on the window.
unsigned int Text::get_posX() const { return this->posX; }
//! Gets the horizontal position of the text on the window.
unsigned int Text::get_posY() const { return this->posY; }
//! Gets the contents of the text.
std::string Text::get_string() const { return this->string; }
//! Gets the color in which the text is rendered.
Color Text::get_color() const { return this->color; }

//! Sets the text position on the window.
void Text::set_pos( unsigned int x, unsigned int y )
{
  set_posX( x );
  set_posY( y );
}
//! Sets the horizontal position of the text on the window.
void Text::set_posX( unsigned int x ) { this->posX = x; }
//! Sets the vertical position of the text on the window.
void Text::set_posY( unsigned int y ) { this->posY = y; };
//! Sets the text contents.
void Text::set_string( const std::string & str ) { this->string = str; }
//! Sets the color in which the text is rendered.
void Text::set_color( const Color & color ) { this->color = color; }

