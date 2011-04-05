#include "sprite.h"

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <SDL_image.h>

#define GIF_DIR "gifs"

static SDL_Thread *sprite_parser_tid;

SPRITE_RV sprite_init(Sprite *spr, char *filename)
{
  fprintf(stderr, "sprite_init : %s\n", filename);

  char frame_filename[256];
  int frame_idx = 0;
  // FILE *fp;

  sprintf(frame_filename, "%s/%s/frame%d.gif", GIF_DIR, filename, frame_idx); 
  fprintf(stderr, "testing file : %s\n", frame_filename);

  while (( (spr->frames[frame_idx] = IMG_Load(frame_filename)) != NULL) && frame_idx < SPRITE_MAX_FRAMES)
    {
      fprintf(stderr, "exists\n");
      //fclose(fp);
      frame_idx++;
      sprintf(frame_filename, "%s/%s/frame%0d.gif", GIF_DIR, filename, frame_idx); 
      fprintf(stderr, "testing file : %s\n", frame_filename);
    }


  spr->current = 0;
  spr->frames_len = frame_idx;

  fprintf(stderr, "count : %d\n", frame_idx);

  return SPRV_OK;
};

SPRITE_RV sprite_init_from_dir(Sprite *spr, char *dirname)
{
  fprintf(stderr, "sprite_init_from_dir : %s\n", dirname);

  char frame_filename[12];
  int frame_idx = 0;
  FILE *fp;

  sprintf(frame_filename, "%s/frame%d.gif", dirname, frame_idx); 
  fprintf(stderr, "testing file : %s\n", frame_filename);

  fprintf(stderr, "fopen\n");

  while (((fp = fopen(frame_filename, "r")) != NULL) && frame_idx < SPRITE_MAX_FRAMES)
    {
      fprintf(stderr, "exists\n");
      ///      
      spr->frames[frame_idx] = IMG_Load(frame_filename);
      if (spr->frames[frame_idx] == NULL)
	fprintf(stderr, "NULL frame after image load %d of file %s\n", frame_idx, frame_filename);
      //fclose(fp);
      frame_idx++;
      sprintf(frame_filename, "%s/frame%0d.gif", dirname, frame_idx); 
      fprintf(stderr, "testing file : %s\n", frame_filename);
    }


  spr->current = 0;
  spr->frames_len = frame_idx;

  fprintf(stderr, "count : %d\n", frame_idx);

  return SPRV_OK;
};

SPRITE_RV sprite_free(Sprite *spr)
{
  for ( spr->current = 0; spr->current < spr->frames_len; spr->current++)
    {
      SDL_FreeSurface(spr->frames[spr->current]);

      char frame_filename[12];
      sprintf(frame_filename, "frame%d.gif", spr->current);
      remove(frame_filename);
    }

  spr->frames_len = 0;

  return SPRV_OK;
};

SDL_Surface *sprite_next(Sprite *spr)
{
  fprintf(stderr, "sprite_next: ENTER\n");
  if (spr->frames_len == 0)
    {
      fprintf(stderr, "sprite_next: frames_len is 0\n");
      return NULL;
    }
  SDL_Surface *cur = spr->frames[spr->current++];
  if (spr->current == spr->frames_len)
    spr->current = 0;

  if (cur == NULL)
    fprintf(stderr, "sprite_next: spr->frames[%d] is NULL!! \n", spr->current-1);

  return cur;
};

static int sprite_parser_thread(void *arg)
{
  for (;;)
    {
      DIR *gifs = opendir(GIF_DIR);
      if (!gifs)
	{
	  fprintf(stderr, "sprite_parser_thread : couldn't open %s dir, exiting\n", GIF_DIR);
	  return 10;
	}
      struct dirent *entry = NULL;
      do
	{
	  entry = readdir(gifs);
	  if ((entry)&&(entry->d_type == DT_REG))
	    {
	      fprintf(stderr, "gifs entry : %s\n", entry->d_name);
	      char frames_name[256], frames_dir[1024];
	      strncpy(frames_name, entry->d_name, strlen(entry->d_name) - 4);
	      sprintf(frames_dir, "%s/%s", GIF_DIR, frames_name);
	      fprintf(stderr, "checkin dir: %s\n", frames_dir);

	      if (mkdir(frames_dir, S_IRUSR | S_IWUSR | S_IXUSR) == 0)
		{
		  fprintf(stderr, "created dir: %s\n", frames_dir);
		  char convert_cmd[1024];
		  sprintf(convert_cmd, "/usr/bin/convert +adjoin -coalesce %s/%s %s/%s", 
			  GIF_DIR, entry->d_name, frames_dir, "frame%0d.gif");

		  int sys_rv = system(convert_cmd);
		  fprintf(stderr, "system : %s       returns %d\n", convert_cmd, sys_rv);		   
		}
	    }
	}
      while (entry != NULL);

      if (gifs)
	closedir(gifs);
      break;
    }

  return 0;
}

int sprite_start_parser_thread()
{
  sprite_parser_tid = SDL_CreateThread(sprite_parser_thread, NULL);
  return 0;
}

int sprite_stop_parser_thread()
{
  return 0;
}
