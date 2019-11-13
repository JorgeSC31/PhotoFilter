#ifndef COMMON_H
#define COMMON_H

#include <SDL2/SDL.h>

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int BUTTON_WIDTH;
extern const int BUTTON_HEIGHT;
extern const int SCROLL_WIDTH;
extern const int SCROLL_HEIGHT;
enum LIconSprite {
    ICON_SPRITE_RESTART,
    ICON_SPRITE_SAVE,
    ICON_SPRITE_INVERSE,
    ICON_SPRITE_BN,
    ICON_SPRITE_SEP,
    ICON_SPRITE_OPAC,
    ICON_SPRITE_FLIP_H,
    ICON_SPRITE_FLIP_V,
    ICON_SPRITE_TOTAL
};

enum LLABELSprite {
    LABEL_SPRITE_INVERSE  = 2,
    LABEL_SPRITE_BN       = 3,
    LABEL_SPRITE_SEP      = 4,
    LABEL_SPRITE_OPAC     = 5,
    LABEL_SPRITE_FLIP_H   = 6,
    LABEL_SPRITE_FLIP_V   = 7,
    LABEL_SPRITE_BRIGHT   = 8,
    LABEL_SPRITE_CH_RED   = 9,
    LABEL_SPRITE_CH_GREEN = 10,
    LABEL_SPRITE_CH_BLUE  = 11,
    LABEL_SPRITE_TOTAL    = 10
};
extern SDL_Renderer* gRenderer;
extern SDL_Window*   gWindow;
// LTexture             spriteIcon;

#endif
