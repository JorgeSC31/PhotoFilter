#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class LTexture {
  public:
    // Constructor
    LTexture();

    // Destructor
    ~LTexture();

    // Carga imagen de una ruta especifica
    bool loadFromFile( std::string path );

    // Crea una textura en blanco
    bool createBlank( int width, int height, SDL_TextureAccess access );

    // Destructor
    void free();

    // Modulación de color
    void setColor( Uint8 red, Uint8 green, Uint8 blue );

    // Modo de fusión
    void setBlendMode( SDL_BlendMode blending );

    // Modulación de Alpha
    void setAlpha( Uint8 alpha );

    // Renderizar en un punto dado
    // x, y es la posición de la ventana donde se va a mostrar la imagen
    // nW, nH es la redimensión de la imagen, default mismo tamaño de la textura
    // clip: posición en la textura, tamaño del rect de la textura a renderizar
    // center: centro del rectangulo para rotar
    // flip: bandera None, Horizontal, Vertical
    void render( int x, int y, int nW = -1, int nH = -1, SDL_Rect* clip = NULL,
                 double angle = 0.0, SDL_Point* center = NULL,
                 SDL_RendererFlip flip = SDL_FLIP_NONE );

    // Dimensiones de la imagen
    int getWidth();
    int getHeight();
    // Obtener si el canal rgb
    bool chActive( int c );

    // manipulación de pixeles
    bool   lockTexture();
    bool   unlockTexture();
    void*  getPixels();
    void   copyPixels( LTexture& src );
    int    getPitch();
    Uint32 getPixel32( unsigned int x, unsigned int y );
    void   contrast();
    void   average();
    void   blackAndWhite();
    void   sepia();
    void   addColor( int v );
    void   bright();
    void   dark();

  private:
    SDL_Texture* mTexture;
    void*        mPixels;
    int          mPitch;
    // Canales rgb activos
    bool chRed, chGreen, chBlue;
    // Dimensiones de imagen
    int mWidth;
    int mHeight;
};

#endif
