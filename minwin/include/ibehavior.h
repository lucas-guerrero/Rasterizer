//!
//! \brief    Declares behaviour interfaces.
//!
//! \file      ibehavior.h
//! \copyright (c) Université d'Artois 2021
//! \author    Tiago de Lima <tiago.delima@univ.artois.fr>
//!

#ifndef _MINWIN_IBEHAVIOR_H_
#define _MINWIN_IBEHAVIOR_H_

namespace minwin {

//! The behaviour of the button quit.
class IButtonBehavior
{
  public:
    virtual ~IButtonBehavior() {};
    //! Action executed when the button is clicked.
    virtual void on_click() const = 0;
};

//! The behavior of a keyboard key.
class IKeyBehavior
{
  public:
    //! Action executed when the key is pressed.
    virtual void on_press() const = 0;
    //! Action executed when the key is released.
    virtual void on_release() const = 0;
};

} // end of namespace minwin

#endif // _MINWIN_IBEHAVIOR_H_
