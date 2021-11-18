//!
//! \brief     Defines class Clock member and related functions.
//!
//! \file      clock.cpp
//! \copyright (c) Université d'Artois 2021
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "clock.h"

using namespace minwin;

//! Constructor.
Clock::Clock()
  : start { SDL_GetTicks() }
{}

//! Gets the elapsed time in milliseconds.
Time Clock::elapsed_time() const
{
  return SDL_GetTicks() - start;
}

//! Restarts the clock.
void Clock::restart()
{
  this->start = SDL_GetTicks();
}

//! Converts a value expressed in Time to seconds.
inline double to_sec( Time t )
{
  return t / 1000.0;
}

