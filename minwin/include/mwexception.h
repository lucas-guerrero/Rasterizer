//!
//! \brief     Declares exception classes.
//!
//! \file      mwexception.h
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _MWEXCEPTION_H_
#define _MWEXCEPTION_H_

#include <exception>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
// Uncomment the lines below if the ones above do not work.
//#include <SDL2/SDL.h>
//#include <SDL2/SDL_ttf.h>

namespace minwin {

//!
//! \brief MinWin Exception (virtual class).
//!
class MWException : std::exception
{
  public:
    virtual const char * what() const noexcept;
};

//!
//! \brief MinWin Font Exception.
//!
//! \details This is thrown when SDL TTF subsystem returns an error,
//!          i.e., when there is something wrong with fonts.
//! 
class MWFontException : MWException
{
  public:
    const char * what() const noexcept override;
};

} // end of namespace minwin

#endif // _MWEXCEPTION_H_

