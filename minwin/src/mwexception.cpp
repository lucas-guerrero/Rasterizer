//!
//! \brief     Defines exception classes member functions.
//!
//! \file      mwexception.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "mwexception.h"

using namespace minwin;

//!
//! \brief The exception message.
//!
//! Gives a hint on what went wrong.
//!
const char* MWException::what() const noexcept
{
  return SDL_GetError();
}

//!
//! \brief The exception message.
//!
//! Gives a hint on what went wrong.
//!
const char* MWFontException::what() const noexcept
{
  return TTF_GetError();
}

