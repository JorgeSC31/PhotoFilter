#include "LButton.h"
#include "LTexture.h"
#include "common.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

// Total de botones
const int TOTAL_BUTTONS = 12;

// Flag de volteo de imagen
SDL_RendererFlip FLIP = SDL_FLIP_NONE;

// Variables globales de brillo y tono
int gBright = 127, gRed = 0xFF, gGreen = 0xFF, gBlue = 0xFF;
// Posiciones de la gora del scroll brillo y tono
SDL_Point posBright, posRed, posGreen, posBlue;

// Textura de imagen
LTexture img, filter, spriteLabel, spriteIcon, spriteScroll, autor;
// Tamaño de recorte de sprite
SDL_Rect gSpriteIconClips[ICON_SPRITE_TOTAL], gSpriteLabelClips[LABEL_SPRITE_TOTAL],
    gSpriteScrollClips[2];

LButton buttons[TOTAL_BUTTONS];

// Inciar SDL
bool init();
// Cargar imagenes
bool loadMedia( char* );
// Inicializa los botones con su posición en pantalla
void initButtons();
// Renderiza el filtro y menu es su posición correcta
void drawMenu();
// Liberar recursos y cerrar SDL
void close();

int main( int argc, char** argv ) {
    // Verificar ruta de imagen
    if ( argc == 1 ) {
        printf( "No se especifico la ruta de la imagen\n" );
        return 0;
    }
    // Start SDL and create Window
    if ( !init() ) {
        printf( "Fallo al iniciar.\n" );
        return -1;
    }

    // Cargar imagenes
    if ( !loadMedia( argv[1] ) ) {
        printf( "Fallo al cargar imagenes.\n" );
        return -1;
    }

    initButtons();

    bool      quit = false;
    SDL_Event ev;

    while ( !quit ) {
        // Procesar eventos
        while ( SDL_PollEvent( &ev ) != 0 ) {
            // Usuario pido salir
            if ( ev.type == SDL_QUIT ) {
                quit = true;
                break;
            }
            int btnPress = -1, mw;
            // Obtener que boton se activo
            if ( ev.type == SDL_MOUSEBUTTONDOWN ) {
                for ( int i = 0; i < ICON_SPRITE_TOTAL; i++ )
                    if ( buttons[i].clickRightEvent( &ev ) )
                        btnPress = i;
            } else if ( ev.type == SDL_MOUSEWHEEL ) {
                for ( int i = ICON_SPRITE_TOTAL; i < TOTAL_BUTTONS; i++ )
                    if ( buttons[i].wheelEvent( &ev ) )
                        btnPress = i;
            }
            switch ( btnPress ) {
                case ICON_SPRITE_RESTART:
                    filter.copyPixels( img );
                    filter.setColor( 0xFF, 0xFF, 0xFF );
                    filter.setAlpha( 0xFF );
                    FLIP    = SDL_FLIP_NONE;
                    gBright = 127;
                    gRed = gGreen = gBlue = 0xFF;
                    posBright.x           = 1103;
                    posRed.x = posGreen.x = posBlue.x = 1167;
                    break;
                case ICON_SPRITE_SAVE: break;
                case LABEL_SPRITE_INVERSE: filter.contrast(); break;
                case LABEL_SPRITE_BN: filter.blackAndWhite(); break;
                case LABEL_SPRITE_SEP: filter.sepia(); break;
                case LABEL_SPRITE_OPAC: filter.average(); break;
                case LABEL_SPRITE_FLIP_H:
                    FLIP = ( SDL_RendererFlip )( FLIP xor SDL_FLIP_HORIZONTAL );
                    break;
                case LABEL_SPRITE_FLIP_V:
                    FLIP = ( SDL_RendererFlip )( FLIP xor SDL_FLIP_VERTICAL );
                    break;
                case LABEL_SPRITE_BRIGHT:
                    mw = ev.wheel.y;
                    // Restar el margen a la posición
                    posBright.x -= 1040;
                    // Se puede modificar el brillo solicitado
                    if ( ( 0 <= posBright.x + mw ) && ( posBright.x + mw <= 127 ) ) {
                        posBright.x += mw;
                        if ( mw > 0 )
                            filter.bright();
                        else
                            filter.dark();
                    }
                    // Subir brillo
                    else if ( mw > 0 )
                        posBright.x = 127;
                    // Bajar brillo
                    else
                        posBright.x = 0;
                    gBright = posBright.x * 2;

                    posBright.x += 1040;
                    break;
                case LABEL_SPRITE_CH_RED:
                    mw = ev.wheel.y;
                    // Restar el margen a la posición
                    posRed.x -= 1040;
                    // Se puede modificar el brillo solicitado
                    if ( ( 0 <= posRed.x + mw ) && ( posRed.x + mw <= 127 ) )
                        posRed.x += mw;
                    // Subir brillo
                    else if ( mw > 0 )
                        posRed.x = 127;
                    // Bajar brillo
                    else
                        posRed.x = 0;
                    gRed = posRed.x * 2;
                    filter.setColor( gRed, gGreen, gBlue );
                    posRed.x += 1040;
                    break;
                case LABEL_SPRITE_CH_GREEN:
                    mw = ev.wheel.y;
                    // Restar el margen a la posición
                    posGreen.x -= 1040;
                    // Se puede modificar el brillo solicitado
                    if ( ( 0 <= posGreen.x + mw ) && ( posGreen.x + mw <= 127 ) )
                        posGreen.x += mw;
                    // Subir brillo
                    else if ( mw > 0 )
                        posGreen.x = 127;
                    // Bajar brillo
                    else
                        posGreen.x = 0;
                    gGreen = posGreen.x * 2;
                    filter.setColor( gGreen, gGreen, gBlue );
                    posGreen.x += 1040;
                    break;
                case LABEL_SPRITE_CH_BLUE:
                    mw = ev.wheel.y;
                    // Restar el margen a la posición
                    posBlue.x -= 1040;
                    // Se puede modificar el brillo solicitado
                    if ( ( 0 <= posBlue.x + mw ) && ( posBlue.x + mw <= 127 ) )
                        posBlue.x += mw;
                    // Subir brillo
                    else if ( mw > 0 )
                        posBlue.x = 127;
                    // Bajar brillo
                    else
                        posBlue.x = 0;
                    gBlue = posBlue.x * 2;
                    filter.setColor( gGreen, gGreen, gBlue );
                    posBlue.x += 1040;
                    break;
            }
        }

        // Limpiar pantalla
        SDL_SetRenderDrawColor( gRenderer, 40, 44, 52, 0xFF );
        SDL_RenderClear( gRenderer );

        // Dibujar pantalla
        drawMenu();

        // Actualizar pantalla
        SDL_RenderPresent( gRenderer );
    }
    // Liberar recursos y cerrar SDL
    close();

    return 0;
}

bool init() {
    // Iniciando SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
        printf( "SDL Video no pudo iniciar. SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    if ( IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG ) == -1 ) {
        printf( "IMG JPG | PNG no pudieron iniciar. SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    // Creando ventana
    gWindow = SDL_CreateWindow( "Photo Editor", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED );
    if ( gWindow == NULL ) {
        printf( "Window no pudo ser creado. SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    // Creando el render para gWindow
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if ( gRenderer == NULL ) {
        printf( "Render no pudo ser creado. SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    // Color de gRenderer rgba
    SDL_SetRenderDrawColor( gRenderer, 40, 44, 52, 0xFF );
    SDL_RenderClear( gRenderer );

    // Iniciando carga JPG
    if ( ( IMG_Init( IMG_INIT_JPG ) & IMG_INIT_JPG ) != IMG_INIT_JPG ) {
        printf( "SDL_image no pudo iniciar. SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    return true;
}

bool loadMedia( char* image ) {
    // Cargar imagenes
    if ( !img.loadFromFile( image ) )
        return false;
    if ( !filter.loadFromFile( image ) )
        return false;
    if ( !spriteLabel.loadFromFile( "Media/spriteLabel.png" ) )
        return false;
    if ( !spriteIcon.loadFromFile( "Media/spriteIcon.png" ) )
        return false;
    if ( !spriteScroll.loadFromFile( "Media/spriteScroll.png" ) )
        return false;
    if ( !autor.loadFromFile( "Media/autor.png" ) )
        return false;

    // Asignar clips de los sprite
    // El sprite Icon viene de en dos columnas
    for ( int i = 0; i < ICON_SPRITE_TOTAL; i++ ) {
        //(i%2)
        gSpriteIconClips[i].x = ( i & 1 ) * BUTTON_WIDTH;
        //(i/2)
        gSpriteIconClips[i].y = ( i >> 1 ) * BUTTON_HEIGHT;
        gSpriteIconClips[i].w = BUTTON_WIDTH;
        gSpriteIconClips[i].h = BUTTON_HEIGHT;
    }
    // El sprite Label viene en una columna
    for ( int i = 0; i < LABEL_SPRITE_TOTAL; i++ ) {
        gSpriteLabelClips[i].x = 0;
        // Largo de la imagen
        gSpriteLabelClips[i].y = i * 20;
        // Ancho de la imagen
        gSpriteLabelClips[i].w = 140;
        gSpriteLabelClips[i].h = 20;
    }
    // Gota de scroll
    gSpriteScrollClips[0] = { 0, 0, 10, 15 };
    // Barra de scroll
    gSpriteScrollClips[1] = { 10, 0, 137, 15 };
    return true;
}

void initButtons() {
    // Textura del boton
    for ( int i = 0; i < ICON_SPRITE_TOTAL; i++ )
        buttons[i].setTexture( &spriteIcon );
    for ( int i = ICON_SPRITE_TOTAL; i < TOTAL_BUTTONS; i++ )
        buttons[i].setTexture( &spriteScroll );
    // Posición del boton
    // El texto empieza en x=1040 y y=20
    // Entre cada bloque el margen es de 20
    // El scroll tiene label y textura en diferente linea
    buttons[ICON_SPRITE_RESTART].setPosition( 1040, 10 );
    buttons[ICON_SPRITE_SAVE].setPosition( 1220, 10 );
    buttons[LABEL_SPRITE_INVERSE].setPosition( 1220, 60 );
    buttons[LABEL_SPRITE_BN].setPosition( 1220, 120 );
    buttons[LABEL_SPRITE_SEP].setPosition( 1220, 180 );
    buttons[LABEL_SPRITE_OPAC].setPosition( 1220, 240 );
    buttons[LABEL_SPRITE_FLIP_H].setPosition( 1220, 300 );
    buttons[LABEL_SPRITE_FLIP_V].setPosition( 1220, 360 );
    // Scroll Buttons
    buttons[LABEL_SPRITE_BRIGHT].setPosition( 1040, 445 );
    buttons[LABEL_SPRITE_CH_RED].setPosition( 1040, 505 );
    buttons[LABEL_SPRITE_CH_GREEN].setPosition( 1040, 565 );
    buttons[LABEL_SPRITE_CH_BLUE].setPosition( 1040, 625 );
    // Scroll posición de gotas
    posBright = { 1103, 445 };
    posRed    = { 1167, 505 };
    posGreen  = { 1167, 565 };
    posBlue   = { 1167, 625 };
}

void drawMenu() {
    int   nW, nH;
    float r;
    nW = filter.getWidth();
    nH = filter.getHeight();
    r  = (float) nW / (float) nH;
    // Nuevo tamaño W y H menor o igual a 1000x700 sin distorción
    if ( nW > 1000 ) {
        nW = 1000;
        nH = (float) nW / r;
    }
    if ( nH > 700 ) {
        nH = 700;
        nW = (float) nH * r;
    }
    // Renderizar filter con margen de 20 y centrado
    filter.render( 20 + 500 - ( nW / 2 ), 20 + 350 - ( nH / 2 ), nW, nH, NULL, 0.0, NULL,
                   FLIP );

    // Renderizar labels
    for ( int i = 0; i < LABEL_SPRITE_TOTAL; i++ ) {
        spriteLabel.render( 1040, 60 + i * 60, 140, 20, &gSpriteLabelClips[i] );
    }

    // Renderizar icono
    buttons[ICON_SPRITE_RESTART].render( &gSpriteIconClips[ICON_SPRITE_RESTART] );
    // buttons[ICON_SPRITE_SAVE].render( &gSpriteIconClips[ICON_SPRITE_SAVE] );
    buttons[LABEL_SPRITE_INVERSE].render( &gSpriteIconClips[ICON_SPRITE_INVERSE] );
    buttons[LABEL_SPRITE_BN].render( &gSpriteIconClips[ICON_SPRITE_BN] );
    buttons[LABEL_SPRITE_SEP].render( &gSpriteIconClips[ICON_SPRITE_SEP] );
    buttons[LABEL_SPRITE_OPAC].render( &gSpriteIconClips[ICON_SPRITE_OPAC] );
    buttons[LABEL_SPRITE_FLIP_H].render( &gSpriteIconClips[ICON_SPRITE_FLIP_H] );
    buttons[LABEL_SPRITE_FLIP_V].render( &gSpriteIconClips[ICON_SPRITE_FLIP_V] );
    // Renderizar scroll
    buttons[LABEL_SPRITE_BRIGHT].render( &gSpriteScrollClips[1] );
    buttons[LABEL_SPRITE_CH_RED].render( &gSpriteScrollClips[1] );
    buttons[LABEL_SPRITE_CH_GREEN].render( &gSpriteScrollClips[1] );
    buttons[LABEL_SPRITE_CH_BLUE].render( &gSpriteScrollClips[1] );
    // Renderizar goras de Scroll
    spriteScroll.render( posBright.x, posBright.y, 10, 15, &gSpriteScrollClips[0] );
    spriteScroll.render( posRed.x, posRed.y, 10, 15, &gSpriteScrollClips[0] );
    spriteScroll.render( posGreen.x, posGreen.y, 10, 15, &gSpriteScrollClips[0] );
    spriteScroll.render( posBlue.x, posBlue.y, 10, 15, &gSpriteScrollClips[0] );
    // Renderizar autor
    autor.render( 1100, 670, 200, 50 );
    // spriteIcon.render( 1040, 100, 100, 100 );
}

void close() {
    // Liberar Texturas cargadas
    img.free();
    filter.free();
    spriteIcon.free();
    spriteLabel.free();
    spriteScroll.free();
    autor.free();

    // Destruir ventana
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow   = NULL;
    gRenderer = NULL;

    // Quitar subsitemas SDL
    IMG_Quit();
    SDL_Quit();
}
