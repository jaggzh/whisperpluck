bin_buttons=whisper-buttons
output_log=buildlog.txt

all: whisper-buttons-run

whisper-buttons-run: whisper-buttons
	./whisper-buttons
	# GTK_DEBUG=interactive ./whisper-buttons

whisper-buttons: main-buttons.c
	gcc \
		-o "$@" \
		$^ \
		`pkg-config --cflags --libs gtk+-3.0` \
		2>&1 | tee "$(output_log)"

vi:
	vi \
		Makefile \
		main-buttons.c \
		"$(output_log)" \
		new.c 
