//!
//! \brief     Defines Window class member functions.
//!
//! \file      window.cpp
//! \copyright (c) 2021 Université d'Artois
//! \author    Tiago de Lima <tiago.delima@univ.artois.fr>
//!

#include "window.h"

using namespace minwin;

//! Constructor.
Window::Window() {}

//! Destructor.
Window::~Window() {}

//! Gets the window title.
inline std::string  Window::get_title() const  { return this->title; }
//! Gets the window width.
inline unsigned int Window::get_width() const  { return this->width; }
//! Gets the window height.
inline unsigned int Window::get_height() const { return this->height; }

//!
//! \brief Gets the current drawing color.
//!
//! Returns the color currently in use by put_pixel().
//!
//! \see put_pixel()
//! \see set_draw_color()
//!
inline Color Window::get_draw_color() const    { return this->draw_color; }

//! Sets the window title.
void Window::set_title( std::string title )     { this->title = title; }
//! Sets the window height.
void Window::set_height( unsigned int height )  { this->height = height; }

//!
//! \brief Sets the window drawing color.
//!
//! This is the color used by put_pixel() to draw pixels on the window surface.
//!
//! \see put_pixel()
//!
//! \param color
//!
void Window::set_draw_color( const Color & color )
{
  SDL_Color c = to_sdl_color( color );
  SDL_SetRenderDrawColor( this->renderer, c.r, c.g, c.b, c.a );
  this->draw_color = color;
}

//!
//! \brief Sets the window width.
//!
//! Also updates the frame rate info position on the window surface.
//!
//! \param width
//!
void Window::set_width( unsigned int width )
{
  this->width = width;
  this->update_fps_info_pos();
}

//!
//! \brief Registers the quit behavior.
//!
//! Registers the behaviour describing what will be executed when the user
//! clicks on the button quit.
//!
//! \see process_input()
//!
//! \param behavior
//!
void Window::register_quit_behavior( IButtonBehavior * const behavior )
{
  this->quit_behavior = behavior;
}

//!
//! \brief Registers a key behavior.
//!
//! Registers the behavior describing what will be executed when the key whose
//! keycode is 'code' is pressed and/or released. 
//!
//! \see process_input()
//!
//! \param code
//! \param behavior
//!
void Window::register_key_behavior( KeyCode code
                                  , IKeyBehavior * const behavior )
{
  this->key_behaviors.emplace( code, behavior );
}

//! Loads a font file.
bool Window::load_font( const std::string & file_name, unsigned int size )
{
  if( not (this->font_initialised) )
  {
    SDL_Log( "Starting font subsystem..." );
    // Initialize SDL_ttf.
    if( TTF_Init() == -1 )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "Failed. %s" , TTF_GetError() );
        SDL_Log( "Text won't be rendered." );
        //throw MWFontException();
        return false;
    }
    this->font_initialised = true;
    SDL_Log( "Font subsystem ready." );
  }

  if( this->font != nullptr )
  {
    SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Font already loaded." );
    return false;
  }

  SDL_Log( "Loading font: %s...", file_name.c_str() );

  this->font = TTF_OpenFont( file_name.c_str(), size );
  if( this->font == nullptr )
  {
      SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                  , "Failed. %s", TTF_GetError() );
      SDL_Log( "Text won't be rendered." );
      //throw MWFontException();
      //this->font_loaded = false;
      return false;
  }

  SDL_Log( "Font loaded." );
  return true;
}

//!
//! \brief Opens the window. 
//!
//! \returns true if everything is OK.
//!
//! \throws MWException If something goes wrong.
//!
bool Window::open()
{
    SDL_Log( "Opening window..." );

    // Initialize SDL.
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "Failed to init SDL. %s", SDL_GetError() );
        throw MWException();
    }

    // Create window.
    this->window = SDL_CreateWindow(
        this->title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        this->width,
        this->height,
        SDL_WINDOW_SHOWN
    );
    if( this->window == nullptr )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "Failed to create window. %s", SDL_GetError() );
        throw MWException();
    }

    // Create renderer for window.
    this->renderer = SDL_CreateRenderer(
        this->window,
        -1,
        //SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
        SDL_RENDERER_ACCELERATED
    );
    if( this->renderer == nullptr )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "Failed to create renderer. %s", SDL_GetError() );
        throw MWException();
    }

    // Initialize renderer color.
    this->set_draw_color( this->draw_color );

    // Set up FPS info.
    this->fps_info.set_string( "0 fps" );
    this->fps_info.set_color( white );
    // Calculate fps info position on the window.
    this->update_fps_info_pos();

    SDL_Log( "Window open." );
    return true;
}

//!
//! \brief Processes keyboard and other inputs.
//!
//! Pools the events from the event pile and executes the corresponding
//! registered behavior.
//! Events for which no behaviour has been registered are discarded.
//!
//! \see register_key_behavior()
//! \see register_quit_behavior()
//!
void Window::process_input()
{
  SDL_Event event;

  // Update delta time, which can then be used by the behaviors below.
  //this->update_delta_time();

  // While there are events to be treated:
  while( SDL_PollEvent( &event ) )
  {
    // If user requested quit:
    if( event.type == SDL_QUIT )
    {
      // Start quit behavior (which stops running the application).
      this->quit_behavior->on_click();
    }
    // If user pressed a key:
    else if( event.type == SDL_KEYDOWN )
    {
      // If key has a registered behavior:
      if( this->key_behaviors[event.key.keysym.sym] != nullptr )
      {
        // Start behavior.
        this->key_behaviors[event.key.keysym.sym]->on_press();
      }
    }
    // if user released a key:
    else if( event.type == SDL_KEYUP )
    {
      // If key has a registered behavior:
      if( this->key_behaviors[event.key.keysym.sym] != nullptr )
      {
        // Stop behavior.
        this->key_behaviors[event.key.keysym.sym]->on_release();
      }
    }
  }

  /*
  // TODO: Discover why it is not working.
  // Get keyboard state.
  const Uint8 * keyboard_state = SDL_GetKeyboardState( NULL );
  // For each registered key state behavior:
  for( auto iter : this->key_state_behaviors )
  {
    // If corresponding key is pressed:
    if( keyboard_state[iter.first] )
    {
      // Start behavior.
      this->key_state_behaviors[iter.first]->start();
    }
    else
    {
      // Stop behavior.
      this->key_state_behaviors[iter.first]->stop();
    }
  }
  */
}

//!
//! \brief Displays window contents rendered so far.
//!
//! Displays contents rendered by clear(), render_text() and put_pixel(),
//! as well as the frame rate.
//!
//! \see clear()
//! \see render_text()
//! \see put_pixel()
//!
void Window::display()
{
  // TODO: Create a class Time to abstract way from time unities.
  static Clock clock;
  static Time  elapsed;

  this->render_fps_info();

  // Update the surface.
  SDL_RenderPresent( this->renderer );

  elapsed = clock.elapsed_time();
  // Wait until min_frame_delay milliseconds has elapsed since last frame.
  if( elapsed < this->min_frame_delay )
  {
    //std::cerr << "Delta: " << delta_ticks << " ms\n";
    //std::cerr << "Sleep: " << this->min_frame_delay - delta_ticks << " ms\n";
    SDL_Delay( this->min_frame_delay - elapsed );
  }
  clock.restart();
}

//!
//! \brief Closes the window.
//!
//! Performs cleaning (unregisters behaviours, free memory, etc.) and closes the
//! window.
//!
void Window::close()
{
  SDL_Log( "Closing window..." );

  // Unregister and destroy all behaviours.
  unregister_behaviors();

  // Free font.
  //if( this->font_loaded )
  if( this->font != nullptr )
  {
    TTF_CloseFont( this->font );
    this->font = nullptr;
  }

  // Destroy window.
  SDL_DestroyRenderer( this->renderer );
  SDL_DestroyWindow( this->window );
  //SDL_FreeSurface( this->surface );
  this->window = nullptr;
  this->renderer = nullptr;
  //this->surface = nullptr;

  // Quit SDL subsystems.
  TTF_Quit();
  SDL_Quit();

  SDL_Log( "Window closed." );
}

//!
//! \brief Clears the window surface.
//!
//! Paints the entire window surface with a specified color and then restores
//! the original drawing color.
//! The result is only visible after calling function display().
//!
//! \see display()
//!
//! \param color Color to be used to paint window surface.
//!
void Window::clear( const Color & color ) const
{
    // Save renderer color.
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor( this->renderer, &r, &g, &b, &a );

    // Specify color to the renderer.
    SDL_Color c = to_sdl_color( color );
    SDL_SetRenderDrawColor( this->renderer, c.r, c.g, c.b, c.a );

    // Clear window with specified color.
    SDL_RenderClear( this->renderer );

    // Restore renderer color.
    SDL_SetRenderDrawColor( this->renderer, r, g, b, a );
}

//! Renders a text on the window surface.
//
// TODO: Optimise text treatment by calculating textures in advance.
//
void Window::render_text( const Text & txt ) const
{
  render_text( txt.get_posX()
             , txt.get_posY()
             , txt.get_string()
             , txt.get_color() );
}

//!
//! \brief Renders a text on the window surface.
//!
//! \deprecated This function may not be present in future releases. Give
//!             preference to render_text( const Text & ) instead.
//!
//! \see render_text( const Text & )
//!
//! \param posX,posY Position on the window.
//! \param text      The text to be rendered.
//! \param color     The color in which the text will be rendered.
//!
//! \throws MWFontException If cannot create texture for text.
//!
void Window::render_text( int posX, int posY
                        , std::string text
                        , const Color & color ) const
{
    // If the font has not been previously loaded, do nothing.
    if( this->font == nullptr )
    {
      //SDL_Log( "Not rendering text: the font is not loaded." );
      return;
    }

    // Render text surface.
    SDL_Surface* text_surface =
      TTF_RenderUTF8_Shaded( this->font
                           , text.c_str()
                           , to_sdl_color( color )
                           , {0,0,0,1} ); // transparent background

    if( text_surface == nullptr )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "The text cannot be rendered: %s", TTF_GetError() );
        throw MWFontException();
    }

    // Create texture from surface pixels.
    SDL_Texture* texture = SDL_CreateTextureFromSurface( this->renderer
                                                       , text_surface );
    if( texture == nullptr )
    {
        SDL_LogError( SDL_LOG_CATEGORY_APPLICATION
                    , "The text texture cannot be created. %s", SDL_GetError() );
        throw MWFontException();
    }

    // Get size of the texture.
	  SDL_Rect src_rect = { 0, 0, text_surface->w, text_surface->h };
    // Set size of the texture on window.
    SDL_Rect dst_rect = { posX, posY, src_rect.w, src_rect.h };

    // Render texture to window.
    SDL_RenderCopy( this->renderer, texture, &src_rect, &dst_rect );

    // Get rid of old surface.
    SDL_FreeSurface( text_surface );
    text_surface = nullptr;
    
    // Free texture.
    SDL_DestroyTexture( texture );
    texture = nullptr;
}

//!
//! \brief Draws one pixel on the window surface.
//!
//! Draws the pixel using current drawing color.
//! The result is only visible after a call to function display().
//!
//! \see display()
//! \see get_draw_color()
//! \see set_draw_color()
//!
//! \param posX Horizontal position of the pixel, i.e., the distance (in pixels)
//!             from the left border of the window.
//! \param posY Vertical position of the pixel, i.e., the distance (in pixels)
//!             from the top border of the window.
//!
void Window::put_pixel( int posX, int posY ) const
{
    SDL_RenderDrawPoint( this->renderer, posX, posY );
}

//!
//! \brief Draws one pixel on the window surface using a specified color.
//!
//! Draws the pixel and then restores the original drawing color.
//! The result is only visible after a call to function display().
//!
//! \see display()
//! \see get_draw_color()
//! \see set_draw_color()
//!
//! \param posX Horizontal position of the pixel, i.e., the distance (in pixels)
//!             from the left border of the window.
//! \param posY Vertical position of the pixel, i.e., the distance (in pixels)
//!             from the top border of the window.
//!
void Window::put_pixel( int posX, int posY, const Color & color ) const
{
    // Save renderer color.
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor( this->renderer, &r, &g, &b, &a );

    // Specify color to the renderer.
    SDL_Color c = to_sdl_color( color );
    SDL_SetRenderDrawColor( this->renderer, c.r, c.g, c.b, c.a );

    this->put_pixel( posX, posY );

    // Restore renderer color.
    SDL_SetRenderDrawColor( this->renderer, r, g, b, a );
}

//!
//! \brief Renders frame rate information.
//!
//! \see update_fps_info_pos()
//!
void Window::render_fps_info()
{
  static unsigned int num_frames; // Number of frames since last restart.
  static Clock clock;

  // If 1 sec has passed since last time the counting started:
  if( clock.elapsed_time() > 1000 )
  {
    // Set the string to be displayed.
    this->fps_info.set_string( std::to_string( num_frames ) + " fps" );
    // Restart counting time and frames.
    clock.restart();
    num_frames = 0u;
  }
  // Update the number of frames.
  ++num_frames;

  // Render frame rate info.
  this->render_text( this->fps_info );
}

//! Converts Color to SDL_Color.
//
// Useful to isolate SDL from other parts of the code.
//
// TODO: This means 4 multiplications per object per frame. Maybe it is better
//       to use integers here?
//
SDL_Color Window::to_sdl_color( const Color & color ) const
{
    //unsigned char r, g, b, a;
    Uint8 r, g, b, a;
    r = color.red * 0xff;
    g = color.green * 0xff;
    b = color.blue * 0xff;
    a = color.alpha * 0xff;

    SDL_Color s = { r, g, b, a };
    return s;
}

//!
//! \brief Updates the position of the fps information text.
//!
//! \see render_fps_info()
//!
void Window::update_fps_info_pos()
{
  if( this->font != nullptr )
  {
    int w, h;
    //TTF_SizeUTF8( font, "999 fps ", &w, &h );
    //fps_info_pos[0] = width - w;
    //fps_info_pos[1] = h + 1;
    TTF_SizeUTF8( font, "999 fps", &w, &h );
    this->fps_info.set_pos( width - w, h + 1 );
  }
}

//!
//! \brief Unregisters and destroys all previously registered behaviors.
//!
//! \see register_quit_behavior()
//! \see register_key_behavior()
//!
void Window::unregister_behaviors()
{
  delete this->quit_behavior;
  this->quit_behavior = nullptr;

  this->key_behaviors.erase( this->key_behaviors.begin()
                           , this->key_behaviors.end() );
}

