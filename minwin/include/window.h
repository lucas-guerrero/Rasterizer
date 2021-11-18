//!
//! \brief     Declares class Window.
//!
//! \file      window.h
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ-artois.fr>
//!

#ifndef _MINWIN_WINDOW_H_
#define _MINWIN_WINDOW_H_

#include <map>
#include <sstream>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "clock.h"
#include "color.h"
#include "mwexception.h"
#include "ibehavior.h"
#include "keycode.h"
#include "text.h"

//!
//! \namespace minwin
//! \brief     MinWin namespace
//!
namespace minwin {

//!
//! \brief An application window.
//!
//! This class contains the window surface where graphics will be rendered.
//! It also controls keyboard and other inputs and handles frame rate.
//!
class Window
{
  public:
    // Constructor
    Window();

    // Destructor
    ~Window();

    // Gets
    inline std::string  get_title() const;
    inline unsigned int get_width() const;
    inline unsigned int get_height() const;
           Color        get_draw_color() const;

    // Sets
    void set_title( std::string );
    void set_width( unsigned int );
    void set_height( unsigned int );
    void set_draw_color( const Color & );

    // Behaviors
    void register_quit_behavior( IButtonBehavior * const );
    void register_key_behavior( KeyCode, IKeyBehavior * const );

    // Fonts
    bool load_font( const std::string & file_name, unsigned int size=14 );

    // Open, close and elements for the application main loop.
    bool open();
    void process_input();
    void clear( const Color & color=black ) const;
    void display();
    void close();

    void render_text( int posX, int posY, std::string txt, const Color & ) const;
    void render_text( const Text & txt ) const;
    void put_pixel( int posX, int posY ) const;
    void put_pixel( int posX, int posY, const Color & ) const;

  private:
    SDL_Window *   window   { nullptr };
    SDL_Renderer * renderer { nullptr };

    std::string  title      { "" };
    unsigned int width      { 1024u };
    unsigned int height     {  768u };
    Color        draw_color { white };

    // Quit behavior.
    IButtonBehavior * quit_behavior { nullptr };

    // Key press-release behaviors container.
    std::map<KeyCode, IKeyBehavior * const> key_behaviors {};

    // Font.
    //static constexpr char default_ttf_file[25] { "fonts/FreeMonoBold.ttf"s};
    static constexpr unsigned int default_font_size { 15u };
    TTF_Font *                    font              { nullptr };
    bool                          font_initialised  { false };

    // Minimum of 16 ms between frames (i.e., maximum of 1000 / 16 = 62.5 fps).
    static constexpr unsigned int min_frame_delay { 16u };

    // If the frame rate is too slow,
    // then do not go further than 0.5 sec in the future.
    //static constexpr double delta_time_limit { 0.5 };

    // Frame rate info.
    Text fps_info;

    //
    // Friends
    //

    // Give access to class Text.
    //friend SDL_Renderer * get_renderer( const Window & );

    //
    // Helper functions
    //

    void render_fps_info();
    SDL_Color to_sdl_color( const Color & ) const;
    void update_fps_info_pos();
    void unregister_behaviors();
    //void update_delta_time();
};

} // end of namespace minwin

#endif // _MINWIN_WINDOW_H_

