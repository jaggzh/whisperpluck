bin_buttons=whisper-buttons

all: whisper-buttons

whisper-buttons: main-buttons.c
	gcc `pkg-config --cflags --libs gtk+-3.0` \
		-o $@ $^

vi:
	vi Makefile main-buttons.c
