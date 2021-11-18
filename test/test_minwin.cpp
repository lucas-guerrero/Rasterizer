//!
//! \brief     MinWin Tester main source file.
//!
//! \file      test_minwin.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#include "test_minwin.h"

// Main function.
int main()
{
  App app;
  app.run();
 
  // return 0 means OK.
  return 0;
}

//! Constructor
App::App() {}

//! Runs application.
void App::run()
{
  // initialise window
  window.set_title( "MinWin Test" );
  window.set_width( 600 );
  window.set_height( 400 );

  // create and register behaviors
  window.register_quit_behavior( new QuitButtonBehavior( *this ) );
  window.register_key_behavior( minwin::KEY_ESCAPE, new QuitKeyBehavior( *this ) );
  window.register_key_behavior( minwin::KEY_SPACE, new ChangeColorBehavior( *this ) );
  
  // setup text
  text1.set_pos( 100, 100 );
  text1.set_string( "Press SPACE to see this in a different color." );
  text1.set_color( this->color );

  text2.set_pos( 190, 120 );
  text2.set_string( "Press ESC to quit." );
  text2.set_color( this->color );

  // load font
  if( not window.load_font( "fonts/FreeMonoBold.ttf", 16u ) )
  {
    std::cerr << "Couldn't load font.\n";
  }

  // open window
  if( not window.open() )
  {
    std::cerr << "Couldn't open window.\n";
    return;
  }

  while( this->running )
  {
    // process keyboard inputs, etc.
    window.process_input();
    
    // clear window
    window.clear();

    // set color
    window.set_draw_color( this->color );
    // draw a line
    for( int i { 10 }; i < 590; ++i )
      window.put_pixel( i, 80 );

    // draw text
    window.render_text( text1 );
    window.render_text( text2 );

    // draw another line
    for( int i { 10 }; i < 590; ++i )
      // set the color for each pixel (it is a bit slower)
      window.put_pixel( i, 150, this->color );
    
    // display elements drawn so far
    window.display();
  }

  // shutdown and close window
  window.close();
}

void App::QuitButtonBehavior::on_click() const { this->owner.running = false; }

void App::QuitKeyBehavior::on_press() const   { this->owner.running = false; }
void App::QuitKeyBehavior::on_release() const {} // does nothing

void App::ChangeColorBehavior::on_press() const
{
  this->owner.color = minwin::green;
  this->owner.text1.set_color( this->owner.color );
  this->owner.text2.set_color( this->owner.color );
}

void App::ChangeColorBehavior::on_release() const
{
  this->owner.color = minwin::white;
  this->owner.text1.set_color( this->owner.color );
  this->owner.text2.set_color( this->owner.color );
}

