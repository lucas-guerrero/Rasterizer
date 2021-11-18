//!
//! \brief     Declares class Clock and related elements.
//!
//! \file      clock.h
//! \copyright (c) 2021 Université d'Artois, see LICENCE file
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "SDL2/SDL.h"
// Uncomment the line below if the one above does not work.
//#include <SDL2/SDL.h>

namespace minwin {

//!
//! \typedef Time
//! \brief Type used for time measures.
//! 
using Time = Uint32;

//!
//! \brief Measures the elapsed time.
//!
class Clock 
{
  public:
    // Constructor.
    Clock();

    // Gets the elapsed time in milliseconds.
    Time elapsed_time() const;

    // Restarts the clock.
    void restart();

  private:
    Time start;
};

// Converts a value expressed in Time to seconds.
inline double to_sec( Time t );

} // end of namespace minwin

#endif // _CLOCK_H_
