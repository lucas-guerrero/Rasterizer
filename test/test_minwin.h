//!
//! \brief     Declares MinWin Tester classes.
//!
//! \file      test_minwin.h
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "color.h"
#include "ibehavior.h"
#include "text.h"
#include "window.h"

class App
{
  public:
    App();
    void run();

  private:
    minwin::Window window;
    minwin::Color  color  { minwin::white };
    minwin::Text   text1;
    minwin::Text   text2;
    bool           running { true };

  class QuitButtonBehavior : public minwin::IButtonBehavior
  {
    public:
      QuitButtonBehavior( App & app ) : owner { app } {}
      void on_click() const override;
    private:
      App & owner;
  };
  
  class QuitKeyBehavior : public minwin::IKeyBehavior
  {
    public:
      QuitKeyBehavior( App & app ) : owner { app } {}
      void on_press() const override;
      void on_release() const override;
    private:
      App & owner;
  };
  
  class ChangeColorBehavior : public minwin::IKeyBehavior
  {
    public:
      ChangeColorBehavior( App & app ) : owner { app } {}
      void on_press() const override;
      void on_release() const override;
    private:
      App & owner;
  };
};

