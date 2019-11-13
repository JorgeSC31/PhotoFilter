#include "LButton.h"

#include "LTexture.h"
#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

LButton::LButton() {
    // Inicializar variables
    mPosition.x    = 0;
    mPosition.y    = 0;
    mCurrentSprite = ICON_SPRITE_RESTART;
    mTexture       = NULL;
}

void LButton::setPosition( int x, int y ) {
    // Asignar posición sup-izq del boton
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::setTexture( LTexture* texture ) {
    // Asignar Textura
    mTexture = texture;
}

void LButton::render( SDL_Rect* clip ) {
    // Mostrar el sprite actual
    // mTexture->render( mPosition.x, mPosition.y, BUTTON_WIDTH, BUTTON_HEIGHT, clip );
    mTexture->render( mPosition.x, mPosition.y, clip->w, clip->h, clip );
}

bool LButton::clickRightEvent( SDL_Event* e ) {
    // Si paso un evento de click mouse
    if ( e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT ) {
        // Obtener cordenadas del mouse
        int x, y;
        x = e->button.x;
        y = e->button.y;
        // Verificar que el click halla sido  dentro del boton
        if ( x >= mPosition.x && x <= mPosition.x + BUTTON_WIDTH && y >= mPosition.y &&
             y <= mPosition.y + BUTTON_HEIGHT ) {
            // click en el boton
            return true;
        }
    }
    return false;
}

bool LButton::wheelEvent( SDL_Event* e ) {
    // Si paso un evento de mousewheel
    if ( e->type == SDL_MOUSEWHEEL ) {
        // Obtener cordenadas del mouse
        int x, y;
        SDL_GetMouseState( &x, &y );
        // Verificar que el click halla sido  dentro del boton
        if ( x >= mPosition.x && x <= mPosition.x + SCROLL_WIDTH && y >= mPosition.y &&
             y <= mPosition.y + SCROLL_HEIGHT ) {
            // click en el boton
            return true;
        }
    }
    return false;
}
