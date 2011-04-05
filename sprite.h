#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "SDL.h"

#define SPRITE_MAX_FRAMES 1000

typedef struct
{
  SDL_Surface *frames[SPRITE_MAX_FRAMES];
  int frames_len;
  int current;
} Sprite;

typedef enum { SPRV_OK = 0, SPRV_NOSUCHFILE } SPRITE_RV;

SPRITE_RV sprite_init(Sprite *spr, char *filename);
SPRITE_RV sprite_init_from_dir(Sprite *spr, char *dirname);
SPRITE_RV sprite_free(Sprite *spr);
SDL_Surface *sprite_next(Sprite *spr);

int sprite_start_parser_thread();
int sprite_stop_parser_thread();

/* #define sprite_current(spr)			\
  spr->frames[spr->current];
*/

#endif //  __SPRITE_H__
