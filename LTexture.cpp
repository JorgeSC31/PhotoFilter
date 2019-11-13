#include "LTexture.h"

#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

LTexture::LTexture() {
    mTexture = NULL;
    mPixels  = NULL;
    mPitch   = 0;
    mWidth   = 0;
    mHeight  = 0;
    chRed = chGreen = chBlue = true;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile( std::string path ) {
    // Desechar la textura acutal
    free();

    // Cargar la imagen desde path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL ) {
        printf( "No se pudo cargar la imagen de %s.\nSDL_image Error: %s\n", path.c_str(),
                IMG_GetError() );
        return false;
    }
    // convertir loadedSurface a formato de pixeles rgba
    loadedSurface = SDL_ConvertSurfaceFormat( loadedSurface, SDL_PIXELFORMAT_ARGB8888, 0 );

    // Crear una textura en blanco
    mTexture =
        SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                           loadedSurface->w, loadedSurface->h );

    SDL_SetTextureBlendMode( mTexture, SDL_BLENDMODE_BLEND );

    // Lock texture for manipulation
    SDL_LockTexture( mTexture, &loadedSurface->clip_rect, &mPixels, &mPitch );

    // Copy loaded/formatted surface pixels
    memcpy( mPixels, loadedSurface->pixels, loadedSurface->pitch * loadedSurface->h );

    // Actualizar mTexture con los pixeles de la imagen.
    SDL_UpdateTexture( mTexture, NULL, loadedSurface->pixels,
                       loadedSurface->w * sizeof( Uint32 ) );

    // Dimensiones de imagen
    mWidth  = loadedSurface->w;
    mHeight = loadedSurface->h;
    unlockTexture();
    mPixels = NULL;

    // Desechar loadedsurface
    SDL_FreeSurface( loadedSurface );
    return true;
}

bool LTexture::createBlank( int width, int height, SDL_TextureAccess access ) {
    // Crear una textura inutilizada
    mTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, access, width, height );
    if ( mTexture == NULL ) {
        printf( "No se pudo crear una textura en blanco.\n SDL ERROR: %s\n", SDL_GetError() );
        return false;
    }
    mWidth  = width;
    mHeight = height;
    return true;
}

void LTexture::free() {
    // mTexture existe
    if ( mTexture != NULL ) {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mPixels  = NULL;
        mPitch   = 0;
        mWidth   = 0;
        mHeight  = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue ) {
    // Modular textura rgb
    // Formula srcC = srcC * (color / 255)
    SDL_SetTextureColorMod( mTexture, red, green, blue );
    if ( red > 0 )
        chRed = true;
    if ( green > 0 )
        chGreen = true;
    if ( blue > 0 )
        chBlue = true;
}

void LTexture::setBlendMode( SDL_BlendMode blending ) {
    // Asignar función de blending
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha ) {
    // Modular Alpha de textura
    SDL_SetTextureAlphaMod( mTexture, alpha );
}

// void LTexture::render(int x, int y, SDL_Rect* clip=NULL, double angle=0.0, SDL_Point*
// center=NULL, SDL_RendererFlip flip=SDL_FLIP_NONE)
void LTexture::render( int x, int y, int nW, int nH, SDL_Rect* clip, double angle,
                       SDL_Point* center, SDL_RendererFlip flip ) {
    // Rect para posición en pantalla y tamaño de imagen
    SDL_Rect dstRect = { x, y, mWidth, mHeight };

    // Asignar la redimensión
    if ( nW != -1 ) {
        dstRect.w = nW;
    }
    if ( nH != -1 ) {
        dstRect.h = nH;
    }

    // Renderizar en pantalla
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &dstRect, angle, center, flip );
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}
bool LTexture::chActive( int c ) {
    // valor valido de color r=0, g=1, b=2
    switch ( c ) {
        case 0: return chRed; break;
        case 1: return chGreen; break;
        case 2: return chBlue; break;
        default: return false; break;
    }
}

bool LTexture::lockTexture() {
    // Textura ya esta bloqueada
    if ( mPixels != NULL ) {
        printf( "La textura ya estaba desbloqueada.\n" );
        return false;
    }
    // Bloquear textura
    if ( SDL_LockTexture( mTexture, NULL, &mPixels, &mPitch ) != 0 ) {
        printf( "No se pudo bloquear la textura.\n SDL ERROR: %s\n", SDL_GetError() );
        return false;
    }
    return true;
}

bool LTexture::unlockTexture() {
    // Textura ya desbloqueada
    if ( mPixels == NULL ) {
        printf( "La textura ya estaba desbloqueada.\n" );
        return false;
    }
    // Desbloquear textura
    SDL_UnlockTexture( mTexture );
    mPixels = NULL;
    mPitch  = 0;
    return true;
}

void* LTexture::getPixels() {
    return mPixels;
}

void LTexture::copyPixels( LTexture& src ) {
    // Textura bloqueada
    lockTexture();
    src.lockTexture();
    if ( mPixels != NULL ) {
        // Copiar a mPixel
        memcpy( mPixels, src.mPixels, mPitch * mHeight );
    }
    unlockTexture();
    src.unlockTexture();
}

int LTexture::getPitch() {
    return mPitch;
}

Uint32 LTexture::getPixel32( unsigned int x, unsigned int y ) {
    // Convertir los pixeles a 32 bit
    Uint32* pixels = (Uint32*) mPixels;
    // Regresar el pixel pedido
    if ( x < 0 || x >= ( mPitch / 4 ) || y < 0 || y >= mHeight )
        return ( 0xFF << 24 ) | ( 0xFF << 16 ) | ( 0xFF << 8 ) | 0xFF;
    return pixels[( y * ( mPitch / 4 ) ) + x];
}

void LTexture::contrast() {
    // Bloquear Textura para editar pixeles
    lockTexture();
    // Formato de pixeles a int byte
    Uint8* pixels = (Uint8*) mPixels;
    for ( int i = 0, fin = ( mPitch * mHeight ); i < fin; i++ ) {
        // Alpha
        if ( i % 4 == 3 ) {
            continue;
        }
        pixels[i] = 0xFF - pixels[i];
    }
    unlockTexture();
}

void LTexture::average() {
    // Bloquear Textura para editar pixeles
    lockTexture();
    // Formato de pixeles a int byte
    Uint32* pixels = (Uint32*) mPixels;
    Uint32  A[9];
    int     r, g, b;
    for ( int i = 0; i < mWidth; i++ ) {
        for ( int j = 0; j < mHeight; j++ ) {
            // Obtener todos los pixeles alrededor
            A[0] = getPixel32( i - 1, j - 1 );
            A[1] = getPixel32( i - 1, j );
            A[2] = getPixel32( i - 1, j + 1 );
            A[3] = getPixel32( i, j - 1 );
            A[4] = getPixel32( i, j );
            A[5] = getPixel32( i, j + 1 );
            A[6] = getPixel32( i + 1, j - 1 );
            A[7] = getPixel32( i + 1, j );
            A[8] = getPixel32( i + 1, j + 1 );
            r = g = b = 0;
            for ( int k = 0; k < 9; k++ ) {
                r += ( A[k] >> 16 ) & 0xFF;
                g += ( A[k] >> 8 ) & 0xFF;
                b += A[k] & 0xFF;
            }
            r /= 9;
            g /= 9;
            b /= 9;
            pixels[( j * ( mPitch / 4 ) ) + i] = ( 0xFF << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
        }
    }
    unlockTexture();
}

void LTexture::blackAndWhite() {
    // Bloquear Textura para editar pixeles
    lockTexture();
    // Formato de pixeles a int byte
    Uint32* pixels = (Uint32*) mPixels;
    Uint8   r, g, b, np;

    for ( int i = 0, fin = ( mPitch / 4 ) * mHeight; i < fin; i++ ) {
        // Obtener valores rgb
        r = ( pixels[i] >> 16 ) & 0xFF;
        g = ( pixels[i] >> 8 ) & 0xFF;
        b = pixels[i] & 0xFF;

        // Modificar pixel a escala de grises
        np = 0.2126f * r + 0.7152f * g + 0.0722f * b;

        pixels[i] = ( 0xFF << 24 ) | ( np << 16 ) | ( np << 8 ) | np;
        // pixels[i] = ( r + g + b ) / 3;
    }
    unlockTexture();
}

void LTexture::sepia() {
    // Bloquear Textura para editar pixeles
    lockTexture();
    // Formato de pixeles a int byte
    Uint32* pixels = (Uint32*) mPixels;
    Uint8   r, g, b;
    int     sr, sg, sb;

    for ( int i = 0, fin = ( mPitch / 4 ) * mHeight; i < fin; i++ ) {
        // Obtener valores rgb
        r = ( pixels[i] >> 16 ) & 0xFF;
        g = ( pixels[i] >> 8 ) & 0xFF;
        b = pixels[i] & 0xFF;

        // Modificar color a sepia
        sr = r * 0.393f + g * 0.769f + b * 0.189f;
        sg = r * 0.349f + g * 0.686f + b * 0.168f;
        sb = r * 0.272f + g * 0.534f + b * 0.131f;
        if ( sr > 255 )
            sr = 255;
        if ( sg > 255 )
            sg = 255;
        if ( sb > 255 )
            sb = 255;
        r = sr;
        g = sg;
        b = sb;

        pixels[i] = ( 0xFF << 24 ) | ( r << 16 ) | ( g << 8 ) | b;
    }
    unlockTexture();
}

void LTexture::addColor( int v ) {
    // Bloquear Textura para editar pixeles
    lockTexture();
    // Formato de pixeles a int byte
    Uint32* pixels = (Uint32*) mPixels;
    Uint8   col[3];

    for ( int i = 0, fin = ( mPitch / 4 ) * mHeight; i < fin; i++ ) {
        // Obtener valores rgb
        col[0] = ( pixels[i] >> 16 ) & 0xFF;
        col[1] = ( pixels[i] >> 8 ) & 0xFF;
        col[2] = pixels[i] & 0xFF;

        // Modificar el valor de color
        for ( int i = 0; i < 3; i++ ) {
            // Bajar brillo mínimo
            if ( (int) col[i] + v < 0 )
                col[i] = 0;
            // Subir brillo máximo
            else if ( (int) col[i] + v > 255 )
                col[i] = 255;
            else
                col[i] += v;
        }

        pixels[i] = ( 0xFF << 24 ) | ( col[0] << 16 ) | ( col[1] << 8 ) | col[2];
    }
    unlockTexture();
}

void LTexture::bright() {
    // Subir el brillo 5 unidades
    addColor( 2 );
}

void LTexture::dark() {
    // Bajar el brillo 5 unidades
    addColor( -2 );
}
