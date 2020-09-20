all:
	gcc -Wall -O0 -g -o siv config.c siv_parametrs.c siv.c dialog_callbacks.c `pkg-config --cflags --libs gtk+-3.0`
