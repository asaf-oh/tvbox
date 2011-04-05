all:
	gcc -I /home/asaf/src/ffmpeg-0.6 -I /usr/include/SDL -lSDL -L /home/asaf/local/lib -lavformat -lavcodec -lswscale -lavdevice -lavutil -lSDL_image cmdutils.c sprite.c main.c