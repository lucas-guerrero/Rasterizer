//!
//! \brief     Declares class Text.
//!
//! \file      text.h
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _MINWIN_TEXT_H_
#define _MINWIN_TEXT_H_

#include <iostream>       // Provides: std::cerr
#include <sstream>        // Provides: std::string
#include "SDL2/SDL.h"     // Provides: SDL_Renderer, SDL_Surface, SDL_Texture
#include "SDL2/SDL_ttf.h" // Provides: TTF_Font

#include "color.h" 

namespace minwin {

class Window;

class Text
{
  public:
    // Constructors.
    Text();
    Text( unsigned int, unsigned int, const std::string &, const Color & );

    // Get
    unsigned int get_posX() const;
    unsigned int get_posY() const;
    std::string get_string() const;
    Color get_color() const;

    // Set
    void set_pos( unsigned int, unsigned int );
    void set_posX( unsigned int );
    void set_posY( unsigned int );
    void set_string( const std::string & );
    void set_color( const Color & );

  private:
    unsigned int posX     { 0 };
    unsigned int posY     { 0 };
    std::string string;
    Color color           { white };

    // TODO: Optimise text treatment by calculating textures in advance.
    //const Font &          font;
    //SDL_Surface * surface { nullptr };
    //SDL_Texture * texture { nullptr };
    //SDL_Renderer * get_renderer( const Window & ) const;
};

} // end of namespace minwin

#endif // _MINWIN_TEXT_H_
