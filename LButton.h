#ifndef LBUTTON_H
#define LBUTTON_H

#include "LTexture.h"
#include "common.h"

#include <SDL2/SDL.h>

class LButton {
  public:
    // Constructor
    LButton();

    // Asignar posición esquina sup-izq
    void setPosition( int x, int y );

    // Asignar Textura
    void setTexture( LTexture* texture );

    // Evaluación de evento
    bool clickRightEvent( SDL_Event* e );
    bool wheelEvent( SDL_Event* e );

    // Muestra sprite del boton
    void render( SDL_Rect* clip );

  private:
    // posición sup-izq
    SDL_Point mPosition;

    // Textura de icono actual
    LIconSprite mCurrentSprite;
    LTexture*   mTexture;
};

#endif
